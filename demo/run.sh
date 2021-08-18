#!/bin/bash

gnome-terminal --command='./client1'
gnome-terminal --command='./client2'
gnome-terminal --command='./client3'
gnome-terminal --command='./nlb' && echo "start Network-load-balancer"
gnome-terminal --command='./source' && echo "start source"
