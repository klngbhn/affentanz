#import "InputGestureView.h"
#import "iOSInputSystem.h"

@implementation InputGestureView

@synthesize inputSystem;

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

- (void)dealloc {
    [super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeTouches && event.subtype == UIEventSubtypeNone) {
        UITouch *aTouch = [touches anyObject];
        CGPoint currentTouchPosition = [aTouch locationInView:self];
        inputSystem->touchBegan(currentTouchPosition.y, -currentTouchPosition.x);
    }
    
    if ([super respondsToSelector:@selector(touchesBegan:withEvent:)]) {
        [super touchesBegan:touches withEvent:event];
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeTouches && event.subtype == UIEventSubtypeNone) {
        UITouch *aTouch = [touches anyObject];
        CGPoint currentTouchPosition = [aTouch locationInView:self];
        inputSystem->touchMoved(currentTouchPosition.y, -currentTouchPosition.x);
    }
    
    if ([super respondsToSelector:@selector(touchesMoved:withEvent:)]) {
        [super touchesMoved:touches withEvent:event];
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeTouches && event.subtype == UIEventSubtypeNone) {
        UITouch *aTouch = [touches anyObject];
        CGPoint currentTouchPosition = [aTouch locationInView:self];
        inputSystem->touchEnded(currentTouchPosition.y, -currentTouchPosition.x);
    }
    
    if ([super respondsToSelector:@selector(touchesEnded:withEvent:)]) {
        [super touchesEnded:touches withEvent:event];
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    if ([super respondsToSelector:@selector(touchesCancelled:withEvent:)]) {
        [super touchesCancelled:touches withEvent:event];
    }
}

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeMotion && event.subtype == UIEventSubtypeMotionShake)
        inputSystem->motionBegan();
    
    if ([super respondsToSelector:@selector(motionBegan:withEvent:)]) {
        [super motionBegan:motion withEvent:event];
    }
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeMotion && event.subtype == UIEventSubtypeMotionShake)
        inputSystem->motionEnded();
    
    if ([super respondsToSelector:@selector(motionEnded:withEvent:)]) {
        [super motionEnded:motion withEvent:event];
    }
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    if(inputSystem && event.type == UIEventTypeMotion && event.subtype == UIEventSubtypeMotionShake)
        inputSystem->motionCancelled();
    
    if ([super respondsToSelector:@selector(motionCancelled:withEvent:)]) {
        [super motionCancelled:motion withEvent:event];
    }
}
@end

