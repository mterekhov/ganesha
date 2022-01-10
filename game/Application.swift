//
//  GGameApplication.swift
//  ganesha
//
//  Created by Mihail Terekhov on 22.07.2021.
//

import Cocoa

class Application: NSApplication {
    
    let applicationDelegate = AppDelegate()
    
    override init() {
        super.init()
        
        delegate = applicationDelegate
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}
