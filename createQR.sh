#!/bin/sh
if [ $# -ge 3 ]
then
#	echo "Genering QR-code with name $2"
	qrencode -m 5 -s11 -l H "$1" -o $2
#	echo "Merging logo & Qr-code into $3.png"
	convert -size 450x450 xc:skyblue $2 -geometry +0+0 -composite Logo_white.png -geometry +154+194 -composite $3.png
	rm $2
#	if [ $? -eq 0 ]
#	then
#		echo 'Success'
#	else
#		echo 'Sadly'
#	fi
else
	echo 'Please set url and filename for QR-code and filename for merged file'
fi
