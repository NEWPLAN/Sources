#!/bin/sh
lstrees() 
{
	if test $# -eq 1
	then 
		lstrees $1 .
	else
		for i in $*
		do
		    if test -d $i;
		    then

			(cd $i 
			for j in *; do
			    lstrees $1 $j
			done)
		    elif test -f $i;
		    then
	
			File_input=$i
			if [ "${File_input##*.}" = "c" -o "${File_input##*.}" = "h" ] ; then
				echo $i
				grep -n $1 $i
				echo "\n"
			fi

		    fi
		done
	fi
}
lstrees $1 $2
