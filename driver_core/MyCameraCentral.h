/*
 macam - webcam app and QuickTime driver component
 Copyright (C) 2002 Matthias Krauss (macam@matthias-krauss.de)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 $Id: MyCameraCentral.h,v 1.1 2002/05/22 04:57:16 dirkx Exp $
 */

#import <Cocoa/Cocoa.h>
#include "GlobalDefs.h"
#import "MyCameraDriver.h"
#import "MyCameraInfo.h"

/*
 This is the plot of MyCameraCentral and the relationship between the Camera driver itself and the client: Consider this as a combined camera database and factory. A client first instantiates a CameraCentral and sets itself as its delegate (the client only has ONE CameraCentral - it's a singleton although this is not enforced but results will be probably terrible if you don't do so). Then the client calls [startup]. This will do two things: First, it will look through all available camera types and ask them for their usb signature (type/vendor id). After that, the wiringThread will be started. This is a thread that looks for usb connections and removals due to hot plugging.

 If a new camera is detected and the device matches one of the camera types, the delegate will be notified by [cameraDetected] with an unique number identifying the camera. The delegate may now reply by using [useCamera] with the id received by the delegate method. If everything is fine, will get a ready-to-use CameraDriver object for the camera - with the delegate already set to the client. But you can change the delegate if you wish. It is also kept in a list of active cameras by the camera centrral.

 If a camera is unplugged, the camera central will detect this from the wiringThread and look in its list of active cameras if that camera is currently in use. If yes, the central will [shutdown] the camera, which will notify the client about it. Atrting at the point of the client notification [cameraHasShutDown], the camera driver object will be useless and can be released by the client. The camera will also notify its center about the [shutdown] which will remove the driver from the list of active drivers and release that object.

 The benefit of this is that client may also simply [shutdown] the camera. It will behave identical to the cameraHasShutDown procedure. The only difference is that the camera central will know that it hasn't been really unplugged and keep the camera in its list of available cameras which can be browsed.

 The way for the client to shut everything down is to call the camera central's [shutdown]. This will stop the wiringThread and shut down all cameras.

 */
@interface MyCameraCentral : NSObject {
    NSMutableArray* cameraTypes;	//A list of dictionaries containing long "vendorID", long "productID" and class "class"
    NSMutableArray* cameras;		//A list of cameras currently connected
/* Why is this ana array and not a dictionary keyed by cid? This will make enumeration simpler - if we implement browsing in the cameras later. On the other hand, finding a cam by cid now requires walking through the array. That's not too bad since that array will probably never be so big...*/

    IBOutlet id delegate;
    BOOL wiringThreadRunning;
    CFRunLoopRef wiringThreadRunLoop;
    NSLock* startupLock;			//the startup lock. see code for details.

    BOOL doNotificationsOnMainThread;		//If our client wants notifications on the main thread
//The following three variables are only used when doNotificationsOnMainThread is YES
    NSConnection* mainThreadConnection;		//connection to wiringThread
    NSConnection* wiringThreadConnection;	//connection to mainThread
    NSRunLoop* mainThreadRunLoop;		//To find out if we're in the main thread

//Localized error messages
    char localizedErrorCStrs[10][256];
    char localizedUnknownErrorCStr[256];
}

//Localization services - we may be in an external application so system services won't work dirctly. Make sure you have an an AutoreleasePool
+ (NSString*) localizedStringFor:(NSString*) str;

//The same for cstr stuff. This one makes an AutoreleasePool on its own
+ (void) localizedCStrFor:(char*)key into:(char*)value;

//A Quicker way - localized error names are cached (for displaying speed purposes)
- (char*) localizedCStrForError:(CameraError)err;

//Init, startup, shutdown, dealloc

- (void) dealloc;
- (BOOL) startupWithNotificationsOnMainThread:(BOOL)nomt;//You should have set the delegate when calling this. Returns success.
- (void) shutdown;	//Stops all cams and stops wiringThread

//Property get/set

- (id) delegate;
- (void) setDelegate:(id)d;
- (BOOL) doesNotificationsOnMainThread;

//Camera management

- (short) numCameras;
- (short) indexOfCamera:(MyCameraDriver*)driver;
- (unsigned long) idOfCameraWithIndex:(short)idx;
- (CameraError) useCameraWithID:(unsigned long)cid to:(MyCameraDriver**)outCam acceptDummy:(BOOL)acceptDummy;
- (MyCameraDriver*) useDummyForError:(CameraError)err;
- (BOOL) getName:(char*)name forDriver:(MyCameraDriver*)driver;
- (BOOL) getName:(char*)name forID:(unsigned long)cid;

//Camera defaults managements
- (BOOL) setCameraToDefaults:(MyCameraDriver*) camera;
- (BOOL) saveCameraSettingsAsDefaults:(MyCameraDriver*) camera;

//wiring stuff
- (void) deviceRemoved:(unsigned long)cid;
- (void) deviceAdded:(io_iterator_t)iterator info:(MyCameraInfo*)info;

//delegate forwarders
- (void) cameraDetected:(unsigned long)cid;

//Notification from running camera
- (void) cameraHasShutDown:(id)sender;

@end
