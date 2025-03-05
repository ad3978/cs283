#!/usr/bin/env bats

# File: student_tests.sh
#
# Create your unit tests suit in this file


@test "Basic command ls" {
    run bash -c "echo 'ls' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Command with arguments ls -la" {
    run bash -c "echo 'ls -la' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "cd to /tmp" {
    run bash -c "echo 'cd /tmp' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "cd with no arguments" {
    run bash -c "echo 'cd' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "exit" {
    run bash -c "echo 'exit' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Empty command" {
    run bash -c "echo '' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"warning: no commands provided"* ]]
}

@test "Basic pipe ls | wc" {
    run bash -c "echo 'ls | wc' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Multi-command pipe ls | grep a | wc -l" {
    run bash -c "echo 'ls | grep a | wc -l' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Maximum allowed pipe commands" {
    run bash -c "echo 'ls | grep a | wc -l | sort | uniq | grep b | wc -l | cat' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Too many piped commands" {
    run bash -c "echo 'ls | grep a | wc -l | sort | uniq | grep b | wc -l | cat | wc' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"error: piping limited to 8 commands"* ]]
}

@test "Command with quoted arguments" {
    run bash -c "echo 'echo \"hello world\"' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello world"* ]]
}

@test "Invalid command execution" {
    run bash -c "echo 'nonexistentcommand' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"execvp"* ]]
}

@test "Multiple sequential commands" {
    run bash -c "printf 'echo first\necho second\n' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"first"* ]]
    [[ "$output" == *"second"* ]]
}

@test "Command with leading spaces" {
    run bash -c "echo '   ls' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Invalid cd to nonexistent directory" {
    run bash -c "echo 'cd /nonexistentdirectory' | ./dsh"
    [ "$status" -eq 0 ]
    [[ "$output" == *"cd"* ]]
}

@test "Return value from pipeline" {
    run bash -c "echo 'ls | grep nonexistentpattern' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Interactive mode exit with EOF" {
    run bash -c "printf '' | ./dsh"
    [ "$status" -eq 0 ]
}

@test "Memory handling for large command" {
    local cmd="echo"
    for i in {1..50}; do
        cmd="$cmd arg$i"
    done
    run bash -c "echo '$cmd' | ./dsh"
    [ "$status" -eq 0 ]
}
