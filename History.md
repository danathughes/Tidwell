Revision History
================

Version 1.0.5
-------------

12 Sept 2018

### New Classes

* SChip8

   A subclass of Chip8 that implements the extended instruction set from SuperChip-8.

### New Features

* Created a Revision History document (History.md) to keep track of version number and major changes.

* Display

** Added methods to scroll the screen down, left and right.

### Improvements

* SimpleSDLGui

** Added frame for display screen, run and step buttons, and program name.

** Allow for changing screen resolution during runtime with up/down arrows.

---


Version 1.0.4
-------------

11 Sept 2018

### New Classes

* SimpleSDLGui

   A simple GUI based on the SDL2 library.  Screen updates are much more responsive, and users use the keyboard directly, as opposed to clicking the keys in the GUI.

### New Features

* Display

** Display can now have arbitrary dimensions, enabiling other Chip-8 like implementations.

** Can dynamically change the Display dimensions during runtime.

---


Version 1.0.3
-------------

10 Sept 2018

### Improvements

* Completely replaced switch / case method of decoding operations with a simpler method of properly masking the opcode and looking up the corresponding operation in the opcode map.

---


Version 1.0.2
-------------

8 Sept 2018

### Improvements

* Implement an operation lookup map in Chip8 class, which maps opcodes to method pointers.

* Modified methods to have a common signature.  All methods now take the address, two register numbers, and a value as arguments.  This greatly simplifies the decoding aspect of the chip.

---


Version 1.0.1
-------------

7 Sept 2018

### Improvements

* Organized code base into core and view subdirectories, separating the core computing components from the various GUIs.

---


Version 1.0.0
-------------

13 July 2018

Major Revision change.  Initial fully functional release.  Chip-8 programs appear to work correctly.

### New Classes

* Clock

   Moved the clock to a separate class.  The purpose of this class is to run clock(s) in a separate thread, and to trigger cycles and sound / delay timer countdowns separately, as opposed to having the GUI do it.

* GtkmmGui

   Converted the GladeGui to use the gtkmm library, and object oriented version of gtkmm.  

### New Features

* Chip8

** Added code to cycle the delay and sound timer.

* GtkmmGui

** Maintain and updates the content of the stack on the GUI.

### Improvements

* Separated code base into src and include directories.

* Refined the update methods of the ChipListener.

* Moved the calls to the update methods in the Chip8 class to specific operations, which removed unneeded calls to the GUI when things do not change.

---


Version 0.2.5
-------------

27 April 2018

### New Features

* GladeGui

** Implemented a file dialog box for loading programs arbitrarily, rather than being hardcoded in the main program.

### Bug Fixes

* Fixed the delay timer and sound timer datasets (converted from 8-bit to 16-bit).

---

 
Version 0.2.4
-------------

26 April 2018

### Bug Fixes

* Fixed a drawing bug in the Display class.

---


Version 0.2.3
-------------

19 April 2018

### New Classes

* ChipListener

   An interface to be implemented by GUIs to allow the chip to communicate changes without needed to know a specific GUI setup.  Basically used to fully decouple the GUI from the Computer.

### New Features

* GladeGui

** Implemented needed update methods to work with the ChipListener interface

* Chip8

** Allow adding a ChipListener object.

** Calls the update methods of the ChipListener after each compute cycle.

---


Version 0.2.2
-------------

13 April 2018

### New Classes

* Computer

   A simple class used to encapsulate all of the components in the system (chip, memory, keyboard, display), manage interaction between these, and act as a single interface to GUIs.

### New Features

* Chip8

** Added methods to interact with the Keyboard class

* Memory

** Implemented a method to provide a pretty string representation of memory contents (complete with addresses every N bytes).

* GladeGui

** Added widgets and callbacks to interact with the Keyboard class, so that user can press the keys.

** Added listener methods to update memory, program counter, stack pointer, and address register widgets, in order to work towards an MPC framework.

* Started using CMake as the build system, and created appropriate directory structure.

* Downloaded and added a whole lot of Chip-8 and Chip-48 programs to the repo.

### Improvements

* Moved all the computer components (Chip, Memory, Display, Keyboard) into the GladeGui class, rather than the Chip class, to avoid needless 

---


Version 0.2.1
-------------

12 April 2018

### New Classes

* GladeGui

   A Class based on the GtkGui, except that the GUI is defined in a glade file in order to decouple the interface design and operation.

### New Features

* GladeGui

** Added a _Run_ button, so programs can run continuously rather than having the user simply step through them.

** Added a widget to draw the display to the GUI.

### Bug Fixes

* Fixed issue with drawing too many lines with the __draw__ operation.  Was passing a byte (up to 256 lines) instead of a nybble (up to 16 lines) to the operation.

---


Version 0.2.0
-------------

05 April 2018

Minor Revision change.  The core functionality of the emulator can be demonstrated.  Sound is currently missing, and there are several bugs and architectural changes to be made.

### New Features

* Chip8

** Implemented the __draw__, __skip key pressed__, __skip key not pressed__, __get key__, __set address sprite__, and __store bcd__ operations

* Diplay

** Added methods to get and set individual pixels, write a line of 8 pixels (for the __draw__ operation), and dump the contents of the display to the console.

* Keyboard

** Implemented methods to check if a key is pressed, and to indicate when a key is pressed and released.

* Memory

** Implmented methods to load sprites (16 hex digits 0-F), and to get the address of specific sprites.

* GtkGui

** Create a window with widgets for displaying register values, program counter and stack pointer

** Correctly destroys window when closed

** Button to perform a single cycle on the chip

---


Version 0.1.2
-------------

04 April 2018

### New Classes

* GtkGui

   A simple GUI implemented using GTK.  

### Improvements

* Chip8

** Added needed getters for register values, address, program counter and stack pointer

---


Version 0.1.1
-------------

27 March 2018

### New Features

* Memory

** Implemented code to load ROM files into memory.

** Added code to Memory class to read and write to memory, dump the contents of memroy to the console, and identify where ROM and display memory is located.

* Chip8

** Implemented a basic decoder using switch / case blocks.

** Implemented most of the microprocessor operations.  Still need to implement __draw__, __skip key pressed__, __skip key not pressed__, __get key__, __set address sprite__, __store bcd__

** Implement ability to simulate a single cycle (instruction fetch, instruction decode, execution).  Can cycle through the _maze_ program (without display).

### Improvements

* Chip8

** Replaced hard-coded call stack size with defined constant

---


Version 0.1.0
-------------

26 March 2018

Initial Version, consisting primarily of skeleton code for core components.  Created a main program to run and test the emulator.

### New Classes

* Chip8

   The main class to simulate microcontroller.

* Keyboard

   An abstraction of keyboard functions.

* Memory

  An abstraction of memory.

* Display

  An abstraction of the display or screen.

---



