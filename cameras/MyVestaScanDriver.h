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
 $Id: MyVestaScanDriver.h,v 1.1 2002/05/22 04:57:14 dirkx Exp $
 */

#import <Cocoa/Cocoa.h>
#import "MyTimonFamilyDriver.h"

@interface MyVestaScanDriver : MyTimonFamilyDriver {

}

+ (unsigned short) cameraUsbProductID;
+ (unsigned short) cameraUsbVendorID;
+ (NSString*) cameraName;

- (CameraError) startupWithUsbDeviceRef:(io_service_t)usbDeviceRef;
    


@end
