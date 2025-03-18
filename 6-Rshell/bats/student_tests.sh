#!/usr/bin/env bats

# Setup function to ensure the server isn't already running
setup() {
    pkill -f "./dsh -s" || true
}

@test "Basic ls command works" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dsh_cli.c" ]]
}

@test "cd command works" {
    mkdir -p /tmp/dsh_test_dir
    run ./dsh <<EOF
cd /tmp/dsh_test_dir
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "/tmp/dsh_test_dir" ]]
    rmdir /tmp/dsh_test_dir
}

@test "Piped commands work" {
    run ./dsh <<EOF
ls | wc -l
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ [0-9]+ ]]
}

@test "Exit command works" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}


@test "Remote shell starts server in background" {

    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1


    ps -p $SERVER_PID
    [ "$?" -eq 0 ]

    kill $SERVER_PID
}

@test "Remote client connects to server" {

    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1
    run ./dsh -c -p 12345 <<EOF
exit
EOF
    [ "$status" -eq 0 ]

    kill $SERVER_PID
}

@test "Remote shell runs ls on remote server" {

    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1

    run ./dsh -c -p 12345 <<EOF
ls
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "dsh_cli.c" ]]

    # Kill server
    kill $SERVER_PID
}

@test "Remote shell runs piped command on remote server" {

    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1

    run ./dsh -c -p 12345 <<EOF
ls | wc -l
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ [0-9]+ ]]

    kill $SERVER_PID
}

@test "Remote shell runs cd on remote server" {

    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1


    mkdir -p /tmp/dsh_remote_test_dir

    run ./dsh -c -p 12345 <<EOF

cd /tmp/dsh_remote_test_dir
pwd
exit
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "/tmp/dsh_remote_test_dir" ]]

    rmdir /tmp/dsh_remote_test_dir
    kill $SERVER_PID
}

@test "Stop server command" {
    ./dsh -s -p 12345 &
    SERVER_PID=$!
    sleep 1


    run ./dsh -c -p 12345 <<EOF
stop-server
EOF
    [ "$status" -eq 0 ]

    ! ps -p $SERVER_PID
}


@test "Built-in command handling" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "/tmp" ]]
}



teardown() {
    pkill -f "./dsh" || true
}
