#import <AppKit/AppKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>
#import <AudioToolbox/AudioToolbox.h>

#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

extern void IOBluetoothPreferenceSetControllerPowerState(int state);
extern void IOBluetoothPreferenceSetDiscoverableState(int state);

#define RILL_WIDTH 640u
#define RILL_HEIGHT 360u
#define RILL_STATE_VERSION 1u
#define RILL_STATE_PATH "/var/db/agar/rill.bits"
#define RILL_WORLD_VERSION 1u
#define RILL_WORLD_PATH "/var/db/agar/world.bits"

struct rill_header {
    uint8_t magic[8];
    uint32_t version;
    uint32_t width;
    uint32_t height;
    uint32_t cell_bytes;
    uint64_t generation;
    uint8_t reserved[32];
};

struct world_field {
    uint8_t magic[8];
    uint32_t version;
    uint32_t width;
    uint32_t height;
    uint64_t light_generation;
    uint64_t sound_generation;
    uint64_t radio_generation;
    uint8_t light[RILL_WIDTH];
    uint8_t sound[RILL_HEIGHT];
    uint8_t radio[RILL_HEIGHT];
};

static const uint8_t world_magic[8] = {'R', 'I', 'L', 'L', 'W', 'R', 'L', '1'};
static const uint8_t rill_magic[8] = {'A', 'G', 'A', 'R', 'B', 'I', 'T', '1'};

@interface RillSurfaceView : NSView
@property(nonatomic, assign) const uint8_t *cells;
@property(nonatomic, strong) NSTimer *refreshTimer;
- (instancetype)initWithFrame:(NSRect)frame cells:(const uint8_t *)cells;
- (void)begin;
@end

@implementation RillSurfaceView

- (instancetype)initWithFrame:(NSRect)frame cells:(const uint8_t *)cells {
    self = [super initWithFrame:frame];
    if (!self) return nil;
    _cells = cells;
    return self;
}

- (BOOL)isOpaque {
    return YES;
}

- (void)begin {
    self.refreshTimer = [NSTimer timerWithTimeInterval:(1.0 / 24.0)
                                                target:self
                                              selector:@selector(refresh:)
                                              userInfo:nil
                                               repeats:YES];
    [[NSRunLoop mainRunLoop] addTimer:self.refreshTimer forMode:NSRunLoopCommonModes];
    [self setNeedsDisplay:YES];
}

- (void)refresh:(NSTimer *)timer {
    (void)timer;
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
    (void)dirtyRect;
    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    if (!context) return;

    CGRect bounds = NSRectToCGRect(self.bounds);
    CGContextSetShouldAntialias(context, false);
    CGContextSetRGBFillColor(context, 0, 0, 0, 1);
    CGContextFillRect(context, bounds);
    if (!self.cells) return;

    CGFloat unit = floor(bounds.size.width / (CGFloat)RILL_WIDTH);
    if (unit < 1) unit = 1;
    CGFloat bandWidth = unit * (CGFloat)RILL_WIDTH;
    CGFloat bandHeight = unit * 8.0;
    CGFloat originX = floor((bounds.size.width - bandWidth) / 2.0);
    CGFloat originY = floor((bounds.size.height - bandHeight) / 2.0);

    CGContextSetRGBFillColor(context, 1, 1, 1, 1);
    const uint8_t *edge = self.cells + ((RILL_HEIGHT - 1u) * RILL_WIDTH);
    for (uint32_t x = 0; x < RILL_WIDTH; ++x) {
        uint8_t state = edge[x];
        for (uint32_t bit = 0; bit < 8; ++bit) {
            if ((state & (uint8_t)(1u << bit)) == 0) continue;
            CGRect point = CGRectMake(originX + ((CGFloat)x * unit),
                                      originY + ((CGFloat)bit * unit),
                                      unit, unit);
            CGContextFillRect(context, point);
        }
    }
}

- (void)dealloc {
    [self.refreshTimer invalidate];
}

@end

@interface RillWorld : NSObject
    <AVCaptureVideoDataOutputSampleBufferDelegate,
     AVCaptureAudioDataOutputSampleBufferDelegate,
     CBCentralManagerDelegate>
@property(nonatomic, strong) AVCaptureSession *session;
@property(nonatomic, strong) AVCaptureVideoDataOutput *videoOutput;
@property(nonatomic, strong) AVCaptureAudioDataOutput *audioOutput;
@property(nonatomic, strong) CBCentralManager *central;
@property(nonatomic) dispatch_queue_t captureQueue;
@property(nonatomic) dispatch_source_t decayTimer;
@property(nonatomic) struct world_field *field;
@property(nonatomic) int descriptor;
@property(nonatomic) int rillDescriptor;
@property(nonatomic) void *rillMapping;
@property(nonatomic) size_t rillMappingSize;
@property(nonatomic, assign) const uint8_t *rillCells;
@property(nonatomic, strong) NSWindow *surfaceWindow;
@property(nonatomic, strong) RillSurfaceView *surfaceView;
@end

@implementation RillWorld

- (instancetype)init {
    self = [super init];
    if (!self) return nil;

    _descriptor = open(RILL_WORLD_PATH, O_RDWR | O_CREAT, 0600);
    _rillDescriptor = -1;
    if (_descriptor < 0) return nil;
    if (ftruncate(_descriptor, (off_t)sizeof(struct world_field)) != 0) return nil;

    void *mapping = mmap(NULL, sizeof(struct world_field),
                         PROT_READ | PROT_WRITE, MAP_SHARED, _descriptor, 0);
    if (mapping == MAP_FAILED) return nil;
    _field = mapping;

    if (memcmp(_field->magic, world_magic, sizeof(world_magic)) != 0 ||
        _field->version != RILL_WORLD_VERSION ||
        _field->width != RILL_WIDTH ||
        _field->height != RILL_HEIGHT) {
        memset(_field, 0, sizeof(*_field));
        memcpy(_field->magic, world_magic, sizeof(world_magic));
        _field->version = RILL_WORLD_VERSION;
        _field->width = RILL_WIDTH;
        _field->height = RILL_HEIGHT;
        msync(_field, sizeof(*_field), MS_SYNC);
    }

    _captureQueue = dispatch_queue_create("local.agar.rill.world.capture", DISPATCH_QUEUE_SERIAL);

    IOBluetoothPreferenceSetDiscoverableState(0);
    IOBluetoothPreferenceSetControllerPowerState(1);
    _central = [[CBCentralManager alloc]
        initWithDelegate:self
        queue:dispatch_get_main_queue()
        options:@{CBCentralManagerOptionShowPowerAlertKey: @NO}];

    _decayTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0,
                                         dispatch_get_main_queue());
    dispatch_source_set_timer(_decayTimer,
                              dispatch_time(DISPATCH_TIME_NOW, 250 * NSEC_PER_MSEC),
                              250 * NSEC_PER_MSEC,
                              25 * NSEC_PER_MSEC);
    __weak RillWorld *weakSelf = self;
    dispatch_source_set_event_handler(_decayTimer, ^{
        RillWorld *strongSelf = weakSelf;
        if (!strongSelf || !strongSelf.field) return;
        for (uint32_t i = 0; i < RILL_HEIGHT; ++i) {
            uint8_t value = strongSelf.field->radio[i];
            strongSelf.field->radio[i] = value > 2 ? (uint8_t)(value - 2) : 0;
        }
        msync(strongSelf.field, sizeof(struct world_field), MS_ASYNC);
    });
    dispatch_resume(_decayTimer);

    [self mapRillState];
    [self presentSurface];
    [self requestPhysicalFields];
    return self;
}

- (void)dealloc {
    if (_field) {
        msync(_field, sizeof(*_field), MS_SYNC);
        munmap(_field, sizeof(*_field));
    }
    if (_descriptor >= 0) close(_descriptor);
    if (_rillMapping && _rillMappingSize) munmap(_rillMapping, _rillMappingSize);
    if (_rillDescriptor >= 0) close(_rillDescriptor);
}

- (void)mapRillState {
    const size_t expectedSize = sizeof(struct rill_header) +
                                ((size_t)RILL_WIDTH * RILL_HEIGHT);
    int descriptor = open(RILL_STATE_PATH, O_RDONLY);
    if (descriptor < 0) return;

    struct stat stateStat;
    if (fstat(descriptor, &stateStat) != 0 ||
        (size_t)stateStat.st_size != expectedSize) {
        close(descriptor);
        return;
    }

    void *mapping = mmap(NULL, expectedSize, PROT_READ, MAP_SHARED, descriptor, 0);
    if (mapping == MAP_FAILED) {
        close(descriptor);
        return;
    }

    const struct rill_header *header = mapping;
    if (memcmp(header->magic, rill_magic, sizeof(rill_magic)) != 0 ||
        header->version != RILL_STATE_VERSION ||
        header->width != RILL_WIDTH ||
        header->height != RILL_HEIGHT ||
        header->cell_bytes != 1) {
        munmap(mapping, expectedSize);
        close(descriptor);
        return;
    }

    self.rillDescriptor = descriptor;
    self.rillMapping = mapping;
    self.rillMappingSize = expectedSize;
    self.rillCells = (const uint8_t *)mapping + sizeof(*header);
}

- (void)presentSurface {
    if (!self.rillCells) return;
    NSScreen *screen = [NSScreen mainScreen] ?: [NSScreen screens].firstObject;
    if (!screen) return;

    NSWindow *window = [[NSWindow alloc]
        initWithContentRect:screen.frame
                  styleMask:NSWindowStyleMaskBorderless
                    backing:NSBackingStoreBuffered
                      defer:NO
                     screen:screen];
    window.opaque = YES;
    window.backgroundColor = NSColor.blackColor;
    window.hasShadow = NO;
    window.ignoresMouseEvents = YES;
    window.releasedWhenClosed = NO;
    window.level = CGWindowLevelForKey(kCGDesktopWindowLevelKey) + 1;
    window.collectionBehavior = NSWindowCollectionBehaviorCanJoinAllSpaces |
                                NSWindowCollectionBehaviorStationary |
                                NSWindowCollectionBehaviorIgnoresCycle;

    RillSurfaceView *surface = [[RillSurfaceView alloc]
        initWithFrame:(NSRect){.origin = NSZeroPoint, .size = screen.frame.size}
               cells:self.rillCells];
    window.contentView = surface;
    self.surfaceWindow = window;
    self.surfaceView = surface;
    [window orderFrontRegardless];
    [surface begin];
}

- (void)requestPhysicalFields {
    dispatch_group_t group = dispatch_group_create();
    __block BOOL lightAllowed = NO;
    __block BOOL soundAllowed = NO;

    dispatch_group_enter(group);
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
        lightAllowed = granted;
        dispatch_group_leave(group);
    }];

    dispatch_group_enter(group);
    [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
        soundAllowed = granted;
        dispatch_group_leave(group);
    }];

    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        [self startCaptureWithLight:lightAllowed sound:soundAllowed];
    });
}

- (void)startCaptureWithLight:(BOOL)lightAllowed sound:(BOOL)soundAllowed {
    AVCaptureSession *session = [[AVCaptureSession alloc] init];
    [session beginConfiguration];

    if (lightAllowed) {
        AVCaptureDevice *camera = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        NSError *error = nil;
        AVCaptureDeviceInput *input = camera ? [AVCaptureDeviceInput deviceInputWithDevice:camera error:&error] : nil;
        if (input && !error && [session canAddInput:input]) {
            [session addInput:input];
            AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
            output.alwaysDiscardsLateVideoFrames = YES;
            output.videoSettings = @{
                (NSString *)kCVPixelBufferPixelFormatTypeKey:
                    @(kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange)
            };
            [output setSampleBufferDelegate:self queue:self.captureQueue];
            if ([session canAddOutput:output]) {
                [session addOutput:output];
                self.videoOutput = output;
            }
        }
    }

    if (soundAllowed) {
        AVCaptureDevice *microphone = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
        NSError *error = nil;
        AVCaptureDeviceInput *input = microphone ? [AVCaptureDeviceInput deviceInputWithDevice:microphone error:&error] : nil;
        if (input && !error && [session canAddInput:input]) {
            [session addInput:input];
            AVCaptureAudioDataOutput *output = [[AVCaptureAudioDataOutput alloc] init];
            output.audioSettings = @{
                AVFormatIDKey: @(kAudioFormatLinearPCM),
                AVLinearPCMBitDepthKey: @32,
                AVLinearPCMIsFloatKey: @YES,
                AVLinearPCMIsBigEndianKey: @NO,
                AVLinearPCMIsNonInterleaved: @NO
            };
            [output setSampleBufferDelegate:self queue:self.captureQueue];
            if ([session canAddOutput:output]) {
                [session addOutput:output];
                self.audioOutput = output;
            }
        }
    }

    [session commitConfiguration];
    self.session = session;
    if (self.videoOutput || self.audioOutput) [session startRunning];
}

- (void)captureOutput:(AVCaptureOutput *)output
 didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
        fromConnection:(AVCaptureConnection *)connection {
    (void)connection;
    if (!self.field) return;

    if (output == self.videoOutput) {
        CVPixelBufferRef pixels = CMSampleBufferGetImageBuffer(sampleBuffer);
        if (!pixels) return;
        CVPixelBufferLockBaseAddress(pixels, kCVPixelBufferLock_ReadOnly);
        size_t width = CVPixelBufferGetWidthOfPlane(pixels, 0);
        size_t height = CVPixelBufferGetHeightOfPlane(pixels, 0);
        size_t stride = CVPixelBufferGetBytesPerRowOfPlane(pixels, 0);
        uint8_t *base = CVPixelBufferGetBaseAddressOfPlane(pixels, 0);
        if (base && width && height) {
            for (uint32_t x = 0; x < RILL_WIDTH; ++x) {
                size_t sourceX = ((size_t)x * width) / RILL_WIDTH;
                unsigned sum = 0;
                for (size_t sample = 0; sample < 8; ++sample) {
                    size_t sourceY = ((sample * 2 + 1) * height) / 16;
                    sum += base[sourceY * stride + sourceX];
                }
                self.field->light[x] = (uint8_t)(sum / 8);
            }
        }
        CVPixelBufferUnlockBaseAddress(pixels, kCVPixelBufferLock_ReadOnly);
        return;
    }

    if (output == self.audioOutput) {
        CMBlockBufferRef block = CMSampleBufferGetDataBuffer(sampleBuffer);
        if (!block) return;
        size_t lengthAtOffset = 0;
        size_t totalLength = 0;
        char *data = NULL;
        if (CMBlockBufferGetDataPointer(block, 0, &lengthAtOffset, &totalLength, &data) != kCMBlockBufferNoErr ||
            !data || totalLength < sizeof(float)) return;

        const float *samples = (const float *)data;
        size_t sampleCount = totalLength / sizeof(float);
        for (uint32_t y = 0; y < RILL_HEIGHT; ++y) {
            size_t index = ((size_t)y * sampleCount) / RILL_HEIGHT;
            if (index >= sampleCount) index = sampleCount - 1;
            float value = samples[index];
            if (!isfinite(value)) value = 0;
            if (value < -1) value = -1;
            if (value > 1) value = 1;
            self.field->sound[y] = (uint8_t)lrintf((value + 1.0f) * 127.5f);
        }
    }
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    if (central.state == CBManagerStatePoweredOn) {
        [central scanForPeripheralsWithServices:nil
                                       options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @YES}];
    }
}

- (void)centralManager:(CBCentralManager *)central
 didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary<NSString *, id> *)advertisementData
                  RSSI:(NSNumber *)RSSI {
    (void)central;
    (void)peripheral;
    if (!self.field) return;

    uint64_t hash = UINT64_C(1469598103934665603);
    BOOL foundBytes = NO;
    for (id value in advertisementData.allValues) {
        if (![value isKindOfClass:[NSData class]]) continue;
        foundBytes = YES;
        NSData *bytes = value;
        const uint8_t *pointer = bytes.bytes;
        for (NSUInteger i = 0; i < bytes.length; ++i) {
            hash ^= pointer[i];
            hash *= UINT64_C(1099511628211);
        }
    }

    if (!foundBytes) hash ^= arc4random();
    int rssi = RSSI.intValue;
    int strength = 200 + rssi * 2;
    if (strength < 1) strength = 1;
    if (strength > 255) strength = 255;
    self.field->radio[hash % RILL_HEIGHT] = (uint8_t)strength;
}

@end

int main(int argc, const char *argv[]) {
    (void)argc;
    (void)argv;
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
        RillWorld *world = [[RillWorld alloc] init];
        if (!world) return 1;
        [NSApp run];
    }
    return 0;
}
