*** Variables ***
${ELF}              ${CURDIR}/../../build/Debug/stm32H7_Board.elf
# Renode has no NUCLEO-H723ZG model; the H753ZI board shares the LED pins
# (PB0, PE1, PB14) and USART3, so it is close enough for these tests.
# The RCC warnings about PLL1GRE/PLL1FRACEN in the log are expected.
${PLATFORM}         @platforms/boards/nucleo_h753zi.repl

*** Keywords ***
Create Nucleo
    Execute Command    mach create "nucleo"
    Execute Command    machine LoadPlatformDescription ${PLATFORM}
    Execute Command    sysbus LoadELF @${ELF}

*** Test Cases ***
LED Turns On After Boot
    Create Nucleo
    Create LED Tester    sysbus.gpioPortB.GreenLED    defaultTimeout=2
    Assert LED State     true

Green LED Blinks At 1 Hz
    Create Nucleo
    Create LED Tester    sysbus.gpioPortB.GreenLED    defaultTimeout=2
    Assert LED Is Blinking    testDuration=5    onDuration=0.5    offDuration=0.5    tolerance=0.05    pauseEmulation=true
