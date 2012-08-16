@echo off

git log --all --pretty=oneline | Find /V /C "#never"

@echo on