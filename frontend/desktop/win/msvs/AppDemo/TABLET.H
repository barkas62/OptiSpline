#ifndef   TABLET_INCLUDED
#define   TABLET_INCLUDED

#include "msgpack.h"
#include <wintab.h>
#define  PACKETDATA	(PK_CURSOR | PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE)
#define  PACKETMODE	0
#include <pktdef.h>

#define  NPACKETQSIZE   64

#endif /* TABLET_INCLUDED */
