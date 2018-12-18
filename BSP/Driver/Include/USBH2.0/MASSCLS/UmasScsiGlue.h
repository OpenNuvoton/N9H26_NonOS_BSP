/* Driver for USB Mass Storage compliant devices
 * SCSI Connecting Glue Header File
 *
 * $Id: scsiglue.h,v 1.4 2000/08/25 00:13:51 mdharm Exp $
 *
 * Current development and maintenance by:
 *   (c) 1999, 2000 Matthew Dharm (mdharm-usb@one-eyed-alien.net)
 *
 * This driver is based on the 'USB Mass Storage Class' document. This
 * describes in detail the protocol used to communicate with such
 * devices.  Clearly, the designers had SCSI and ATAPI commands in
 * mind when they created this document.  The commands are all very
 * similar to commands in the SCSI-II and ATAPI specifications.
 *
 * It is important to note that in a number of cases this class
 * exhibits class-specific exemptions from the USB specification.
 * Notably the usage of NAK, STALL and ACK differs from the norm, in
 * that they are used to communicate wait, failed and OK on commands.
 *
 * Also, for certain devices, the interrupt endpoint is used to convey
 * status of a command.
 *
 * Please see http://www.one-eyed-alien.net/~mdharm/linux-usb for more
 * information about this driver.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SCSIGLUE_H_
#define _SCSIGLUE_H_


#define USB_STOR_SCSI_SENSE_HDRSZ      4
#define USB_STOR_SCSI_SENSE_10_HDRSZ   8

struct usb_stor_scsi_sense_hdr
{
    UINT8   *dataLength;
    UINT8   *mediumType;
    UINT8   *devSpecParms;
    UINT8   *blkDescLength;
};

typedef struct usb_stor_scsi_sense_hdr Usb_Stor_Scsi_Sense_Hdr;

union usb_stor_scsi_sense_hdr_u
{
    Usb_Stor_Scsi_Sense_Hdr hdr;
    UINT8   *array[USB_STOR_SCSI_SENSE_HDRSZ];
};

typedef union usb_stor_scsi_sense_hdr_u Usb_Stor_Scsi_Sense_Hdr_u;

struct usb_stor_scsi_sense_hdr_10
{
    UINT8   *dataLengthMSB;
    UINT8   *dataLengthLSB;
    UINT8   *mediumType;
    UINT8   *devSpecParms;
    UINT8   *reserved1;
    UINT8   *reserved2;
    UINT8   *blkDescLengthMSB;
    UINT8   *blkDescLengthLSB;
};

typedef struct usb_stor_scsi_sense_hdr_10 Usb_Stor_Scsi_Sense_Hdr_10;

union usb_stor_scsi_sense_hdr_10_u
{
    Usb_Stor_Scsi_Sense_Hdr_10 hdr;
    UINT8   *array[USB_STOR_SCSI_SENSE_10_HDRSZ];
};

typedef union usb_stor_scsi_sense_hdr_10_u Usb_Stor_Scsi_Sense_Hdr_10_u;

void usb_stor_scsiSenseParseBuffer( SCSI_CMD_T *, Usb_Stor_Scsi_Sense_Hdr_u *,
                                    Usb_Stor_Scsi_Sense_Hdr_10_u *, INT *);


extern UINT8  usb_stor_sense_notready[18];
extern Scsi_Host_Template usb_stor_host_template;
extern INT  usb_stor_scsiSense10to6(SCSI_CMD_T*);
extern INT  usb_stor_scsiSense6to10(SCSI_CMD_T*);

#endif
