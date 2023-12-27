//
//  ViewController.swift
//  doom
//
//  Created by cipher on 11.12.2023.
//

import Cocoa
import MetalKit

class ViewController: NSViewController, MTKViewDelegate {
    
    private let ganeshaBridge = CPPBridge()
    private var ganeshaReady = false

    override var acceptsFirstResponder: Bool {
        get {
            return true
        }
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()

        CGDisplayMoveCursorToPoint(CGMainDisplayID(), screenCenter() ?? NSZeroPoint);
        let trackArea = NSTrackingArea(rect: view.frame, options: [.mouseMoved, .activeInActiveApp], owner: self);
        view.addTrackingArea(trackArea);
//        CGDisplayHideCursor(CGMainDisplayID());
        
        ganeshaBridge.launchEngine(with: view.layer)
        ganeshaReady = true
    }
    
    override func viewDidDisappear() {
        super.viewDidDisappear()
        ganeshaBridge.stopEngine()
    }
    
    override func keyDown(with event: NSEvent) {
        ganeshaBridge.processKeyboardEvent(withKeyCode: event.keyCode);
    }
    
    override func mouseMoved(with event: NSEvent) {
        guard let screenCenterPoint = screenCenter(),
              var mousePoint = view.window?.convertPoint(toScreen: event.locationInWindow) else {
            return
        }
        mousePoint.y += 1;
        
        var diffRange = -0.5...0.5;
        let diff_x = (screenCenterPoint.x - mousePoint.x) / view.frame.width;
        let diff_y = (screenCenterPoint.y - mousePoint.y) / view.frame.height;
        print("diff \(diff_x) - \(diff_y)");
        ganeshaBridge.processMouseMove(withDiffX: diff_x, diff_y: diff_y);
//        if !diffRange.contains(diff_x) || !diffRange.contains(diff_y) {
//            print("return to center")
//            CGDisplayMoveCursorToPoint(CGMainDisplayID(), screenCenterPoint);
//        }
    }
    
    func screenCenter() -> NSPoint? {
        return view.window?.convertPoint(toScreen: NSMakePoint(view.frame.width / 2.0, view.frame.height / 2.0));
    }

    //  MARK: - MTKViewDelegate -

    func draw(in view: MTKView) {
        if !ganeshaReady {
            return
        }
        
        ganeshaBridge.drawFrame()
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        if !ganeshaReady {
            return
        }

        ganeshaBridge.drawableSizeWillChange(size)
    }

}

