#The game
Embark on an adventure in a vast universe with unlimited possibilites.

#Setup projects
Two projects in VS
- Client
- Server

Client properties
----------------------
C++ -> general Aditional include diretories: 
C:\SFML-2.5.1\include;C:\Users\Mathi\source\repos\Asteriods-multiplayer\Server

C++ -> pre-processor definitions: 
debug: PACKAGE_LOSS;

Linker -> general -> additional dependecies
C:\SFML-2.5.1\lib

Linker -> input
debug: winmm.lib;wsock32.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-network.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
release: winmm.lib;wsock32.lib;sfml-main.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-network.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

Server properties
--------------------
c++ -> preprocessor definitions
debug: PACKAGE_LOSS; FAKE_LAG;

linker -> input
debug: winmm.lib;wsock32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)
release: winmm.lib;wsock32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)

Common files
------------------
server: common.cpp
server: common_net.cpp


Solution properties
-----------------------------
Multiple startup projects
1. Server
2. Client




