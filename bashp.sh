if [ $# -eq 1 ]
then
    c=$1;
    while  read -r linie;
    do
    echo "$linie" | grep -E "^[A-Z][A-Za-z0-9 ,.!?]*[.!?]$" | grep -v ",si" | grep -E -c $c
    if [ $? -eq 0 ] 
    then
        ((nr_prop++))
    fi
    done
    echo $nr_prop
else
    echo "numar invalid de argumente"
fi 



