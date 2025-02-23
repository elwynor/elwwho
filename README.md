<pre>
  _       ____         _          ____
 | |     / / /_  ____ ( )_____   / __ \____
 | | /| / / __ \/ __ \|// ___/  / / / / __ \
 | |/ |/ / / / / /_/ / (__  )  / /_/ / / / /
 |__/|__/_/ /_/\____/ /____/   \____/_/ /_/
                                                                
                version 0.1
  Copyright (c) 2025 Elwynor Technologies
</pre>

## WHAT IS WHO'S ON?

 Who's On is a simple who's online global command module that allows the
 SysOp to define up to 5 strings to filter out from the standard Who's On
 listing. This could be to eliminate certain high-use games (MajorMUD) from
 the general user listing. The module also allows users to replace the 
 filters with a string of their choosing, or augment the system filter with
 a string of their choosing. There are up to 4 listing styles to customize!
 
## INSTALLATION AND CONFIGURATION
 
 Simply unzip the archive to your BBS server directory. If you'd like
 the module to work as a menu item, you can add it to your menu. But
 it should work just fine as a global-only product.

## MODULE HISTORY
 
 Who's On was written by Rick Hadsall of Elwynor Technologies in 
 January and February 2025 in response to a request from Brian Morgan
 of The Penalty Box BBS, who wanted a Who's On listing that filtered out
 certain modules due to significant users of MUD games. Rick wrote it
 to work with WG2, WG32, and BBSV10.
 
## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for 
 more details.

## CONTRIBUTING

 We welcome contributions from the community. By contributing, you agree to the
 terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be licensed 
 under the AGPL v3, assign all right, title, and interest, including all 
 copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and 
 you must include the additional terms from the LICENSE file in your project's 
 LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWWHO" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release. To compile for Worldgroup 2 for DOS, use the MKV2P
 utility ("MKV2P ELWWHO") if you have that environment setup.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWWHO.ZIP. When unzipped in a Worldgroup 3.2 or Major BBS V10 installation 
 folder, it "installs" the module. If you compile for DOS, make sure to 
 swap out the compiled DLL file.
