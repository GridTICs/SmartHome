Model Bulb {
    Bool state
    String imagePath
}

RuleContext BulbContext{
    Rule onBulbStateChanged listen (state from Bulb as bulb){
        if (bulb.state){
            bulb.imagePath = bulbOnImagePath
        }else{
            bulb.imagePath = bulbOffImagePath
        }
    }
}


Application {

    String bulbOnImagePath 
    String bulbOffImagePath
    
    String commandChannel
    String stateChannel
    
    Bulb b1
    Bulb b2
    Bulb b3
    
    Main m
    
    OnInit {
        bulbOnImagePath  ="https://dl.dropboxusercontent.com/s/4n7q97nyyqfu935/bombillaON.png"//https://i.imgur.com/OCHptxX.png"
        bulbOffImagePath = "https://i.imgur.com/OCHptxX.png" //"https://i.imgur.com/kq7Yfmg.png"
        
        commandChannel = "command"
        stateChannel = "home"
        
        b1  = Bulb(state:false,imagePath:bulbOffImagePath)
        b2  = Bulb(state:false,imagePath:bulbOffImagePath)
        b3  = Bulb(state:false,imagePath:bulbOffImagePath)
        
        
        m = Main()
        m.light1 = b1.imagePath //Bulb(imagePath:bulbOffImagePath)
        m.light2 = b2.imagePath //Bulb(imagePath:bulbOffImagePath)
        m.light3 = b3.imagePath //Bulb(imagePath:bulbOffImagePath)
        m.logo = "https://dl.dropboxusercontent.com/s/frupwj51i0getz6/arduinoDayLogo.png"
        broker.ui.push("Main",m)
        lightControllerModule.initialize()

    }
}

Entry Experience Main {

    String logo as Image
    String desc1Lbl value("Temperatura:") as Label
    String tempLbl value("Temp") as Label
    String desc2Lbl value("Humedad:") as Label
    String humLbl value("Hum") as Label
    String desc3Lbl value("Intensidad de luz:") as Label
    String ambientLbl value("lux") as Label
    String desc4Lbl value("Última actualización:") as Label
    Date date as Label 
    
    String light1Lbl value("Led 1:") as Label
    String light1 as Image
    Decision light1Btn action("MainContext.onLight1Pressed") label("ON/OFF")
    String light2Lbl value("Led 2:") as Label
    String light2 as Image
    Decision light2Btn action("MainContext.onLight2Pressed") label("ON/OFF")
    String light3Lbl value("Led 3:") as Label
    String light3 as Image
    Decision light3Btn action("MainContext.onLight3Pressed") label("ON/OFF")
    
}

RuleContext MainContext {
    Rule onLight1Pressed{
        lightControllerModule.toggleBulb(1)
    }
    
    Rule onLight2Pressed{
        lightControllerModule.toggleBulb(2)
    }
    
    Rule onLight3Pressed{
        lightControllerModule.toggleBulb(3)
    }

}