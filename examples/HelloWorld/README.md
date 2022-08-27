# HelloWorld example
## How to get started?
1. Select a TI SimpleLink device compatible with the driver.
2. Import the empty example compatible with this device in the CCS workspace
   (Project > Import CCS Projects...).
3. In the CCS project create the folder "NeoPixel". Copy all the content of
   the "source" folder of this repository into the folder newly created.
   Make sure to also include the folder called "SC".
4. Configure the toolchain to consider the newly created folder when building
   by adding it to the search path of the compiler (right click on the 
   project > Properties > Build > Arm Compiler > Include Options, then add
   the entry ``${PROJECT_LOC}/NeoPixel``.
5. In the CCS project, delete the file ``empty.c``.
6. Add the file ``helloWorld.c`` to the project. 
7. Build the project and flash the device.

## Example evaluation
The NeoPixel data PIN should be connected to DIO10 of the LaunchPad.
