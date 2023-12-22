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

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    override var acceptsFirstResponder: Bool {
        get {
            return true
        }
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()

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

