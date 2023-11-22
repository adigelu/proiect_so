#!/bin/bash

functie()
{
    caracter=$1;
    while  read -r line;
    do
	
	((count++))
    done
    echo $count
}
functie $1
    
