//
//  CPPBridge.m
//  ganesha
//
//  Created by Mihail Terekhov on 22.07.2021.
//

#import "CPPBridge.h"
#import <ganesha/ganesha.h>

@interface CPPBridge ()

@property (nonatomic, assign) spcGaneshaEngine::GGanesha *ganeshaEngine;

@end

@implementation CPPBridge

- (void)dealloc {
    self.ganeshaEngine->destroyVulkan();
    delete self.ganeshaEngine;
}

- (void)launchEngineWithLayer:(nullable CALayer *) layer {
    if (!layer) {
        NSLog(@"ACHTUNG: layer is nil");
        return;
    }
    
    self.ganeshaEngine = new spcGaneshaEngine::GGanesha();
    if (!self.ganeshaEngine->initEngine((__bridge void *)layer,
                                       static_cast<uint32_t>(CGRectGetWidth(layer.bounds)),
                                       static_cast<uint32_t>(CGRectGetHeight(layer.bounds)))) {
        NSLog(@"ACHTUNG: no chance to create VULKAN instance");
        return;
    }
}

- (void)stopEngine {
    self.ganeshaEngine->destroyVulkan();
}

- (void)drawableSizeWillChange: (CGSize)size {
    
}

- (void)drawFrame {
    self.ganeshaEngine->mainLoop();
}

- (void)processKeyboardEventWithKeyCode: (NSInteger)keyCode {
    self.ganeshaEngine->processKeyboard(keyCode);
}

@end
