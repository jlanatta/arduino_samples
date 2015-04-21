//
//  ViewController.m
//  RGB Led Control
//
//  Created by Javier Lanatta on 4/21/15.
//  Copyright (c) 2015 Javier Lanatta. All rights reserved.
//

#import "ViewController.h"


@interface ViewController () <NSStreamDelegate>
@property (weak, nonatomic) IBOutlet UILabel *redLabel;
@property (weak, nonatomic) IBOutlet UISlider *redSlider;
@property (weak, nonatomic) IBOutlet UILabel *greenLabel;
@property (weak, nonatomic) IBOutlet UISlider *greenSlider;
@property (weak, nonatomic) IBOutlet UILabel *blueLabel;
@property (weak, nonatomic) IBOutlet UISlider *blueSlider;

@property (strong, nonatomic) NSInputStream *inputStream;
@property (strong, nonatomic) NSOutputStream *outputStream;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)sliderChanged:(UISlider *)sender {
    [self updateLabels];
    [self sendCommand];
}

- (void)updateLabels {
    [[self redLabel] setText:[NSString stringWithFormat:@"Red: %d", (int) self.redSlider.value]];
    [[self greenLabel] setText:[NSString stringWithFormat:@"Green: %d", (int) self.greenSlider.value]];
    [[self blueLabel] setText:[NSString stringWithFormat:@"Blue: %d", (int) self.blueSlider.value]];
}

- (void)sendCommand {
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"192.168.1.177", 1000, &readStream, &writeStream);
    
    self.inputStream = (__bridge_transfer NSInputStream *)readStream;
    self.outputStream = (__bridge_transfer NSOutputStream *)writeStream;
    [self.inputStream setDelegate:self];
    [self.outputStream setDelegate:self];
    [self.inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [self.inputStream open];
    [self.outputStream open];
}

- (void)stream:(NSStream *)stream handleEvent:(NSStreamEvent)eventCode {
    switch(eventCode) {
        case NSStreamEventHasSpaceAvailable:
        {
            NSLog(@"NSStreamEventHasSpaceAvailable!");
            if (stream == self.outputStream) {
                NSString * str = [NSString stringWithFormat:@"%d;%d;%d\n", (int)self.redSlider.value, (int)self.greenSlider.value, (int)self.blueSlider.value];
                const uint8_t * rawstring = (const uint8_t *)[str UTF8String];
                [self.outputStream write:rawstring maxLength:strlen(rawstring)];
                [self.outputStream close];
            }
            break;
        }
        default:
        break;

    }
}

@end
