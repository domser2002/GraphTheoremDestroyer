ps aux | grep ./main | awk '{print $2}' | xargs kill
ps aux | grep main.py | awk '{print $2}' | xargs kill
cd ..
make
./build/bin/main &
cd gtd_frontend
python3 main.py &
cd ../gtd_test
pytest e2e_tests.py