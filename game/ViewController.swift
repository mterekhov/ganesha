//
//  ViewController.swift
//  ganesha
//
//  Created by Mihail Terekhov on 22.07.2021.
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
    
    override func viewDidAppear() {
        super.viewDidAppear()

        ganeshaBridge.launchEngine(with: view.layer)
        ganeshaReady = true
    }
    
    override func viewDidDisappear() {
        super.viewDidDisappear()
        
        ganeshaBridge.stopEngine()
    }
    
    override var acceptsFirstResponder: Bool {
        get {
            return true
        }
    }
    
    override func keyDown(with event: NSEvent) {
        switch event.keyCode {
        case 12:
            print("q")
        case 13:
            print("w")
        case 14:
            print("e")
        case 1:
            print("s")
        case 0:
            print("a")
        case 2:
            print("d")
        default:
            break
        }
    }
    
    //  MARK: - MTKViewDelegate -

    func draw(in view: MTKView) {
        if !ganeshaReady {
            return
        }
        
        ganeshaBridge.drawFrame()
    }
    
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        ganeshaBridge.drawableSizeWillChange(size)
    }

}

