#!/bin/sh
# /etc/swupdate/create_swupdate_cfg.sh ATCProject Marcel Ochsendorf marcel.ochsendorf.com
echo "-- CREATE SWUPODATE CFG---"

#GET MAC ADDRESS
HWID=$(cat /sys/class/net/eth0/address)
#REMOVE THE : OF THE MAC ADDRESS TO GET A CLEANED ONE
HWIDCLEANED=${HWID//:/}
echo "$HWIDCLEANED" > "/HWID"

#GET UPDATE SELECTION ID

BOOTPART=$(cat /etc/swupdate/BOOTPART)
SWVERSION=$(cat /VERSION)
#CHECK IF AN swupdate.cfg already exists
FILEA=/etc/swupdate/SWUPDATE_CFG_GENERATED
if test -f "$FILEA"; then
    	echo "$FILEA exists."
    	echo "swupdate.cfg already created"
else
	#IF NOT CREATE ONE
	
	

    	DEVICEID="ATCTABLE_${HWIDCLEANED}"
    	echo "USING $HWID ($HWIDCLEANED) AS DEVICEID $DEVICEID"
    	#COPY THE TEMPLATE FILE AND REPLACE THE DEVICEID WITH THE GENERATED DEVICEID TO GET AN UNIQUE ID
    	cp /etc/swupdate/swupdate_template.cfg /etc/swupdate/swupdate.cfg
    	sed -i 's|DEVICEID|'"$DEVICEID"'|g' /etc/swupdate/swupdate.cfg
    	
    	sed -i 's|BOOTPART|'"$BOOTPART"'|g' /etc/swupdate/swupdate.cfg
    	sed -i 's|SWVERSION|'"$SWVERSION"'|g' /etc/swupdate/swupdate.cfg
    	touch /etc/swupdate/SWUPDATE_CFG_GENERATED
fi



