while true; do
 ./maker > input
 ./right < input >right.output
 ./wrong < input >wrong.output
 diff right.output wrong.output
 if [ $? -ne 0 ] ; then break; fi
done
