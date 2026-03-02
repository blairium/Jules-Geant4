#!/bin/bash
docker rm -f test_container || true
docker create --name test_container ubuntu:20.04 sleep infinity
docker start test_container
docker exec test_container bash -c "echo 'Hello World'"
docker rm -f test_container
