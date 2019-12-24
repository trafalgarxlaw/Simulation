#!/usr/bin/env bats

# Temporary directory for tests

mkdir -p "./tmp"
BATS_TMPDIR="./tmp"
EXEC="bin/automaton"

@test "Default program working" {
  run "$EXEC"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "Step 0" ]
}

@test "Wrong row value" {
  run "$EXEC" -r a
  [ "$status" -eq 1 ]
  [ "${lines[0]}" = "Error: the number of rows, columns and steps must be positive integers." ]
}

@test "Wrong column value" {
  run "$EXEC" -r 5 -c -4
  [ "$status" -eq 1 ]
  [ "${lines[0]}" = "Error: the number of rows, columns and steps must be positive integers." ]
}

@test "Wrong simulation type" {
  run "$EXEC" -t gameoflife
  [ "$status" -eq 2 ]
  [ "${lines[0]}" = "Error: unrecognized simulation type." ]
}

@test "Type alone is not sufficient" {
  run "$EXEC" -t game-of-life
  [ "$status" -eq 6 ]
  [ "${lines[0]}" = "Error: The type and the allowed cells must be set at the same time." ]
}

@test "Game of life with allowed cells" {
  run "$EXEC" -t game-of-life -a ab
  [ "$status" -eq 0 ]
}

@test "Game of life: inconsistent number of cells" {
  run "$EXEC" -t game-of-life -a abc
  [ "$status" -eq 6 ]
  [ "${lines[0]}" = "Error: The simulation and the allowed cells are inconsistent." ]
}

@test "Wrap-around is not a boundary" {
  run "$EXEC" -b wrap-around
  [ "$status" -eq 3 ]
}

@test "Truncate is a valid boundary" {
  run "$EXEC" -b truncate
  [ "$status" -eq 0 ]
}

@test "Periodic is a valid boundary" {
  run "$EXEC" -b periodic
  [ "$status" -eq 0 ]
}

@test "Wrong distribution" {
  run "$EXEC" -d 1,a
  [ "$status" -eq 4 ]
}

@test "Wrong option" {
  run "$EXEC" -x
  [ "$status" -eq 7 ]
}

@test "Program with initial state" {
  run "$EXEC" -t pandemy -a .XH --stdin < etat.txt
  [ "$status" -eq 0 ]
}

@test "Wrong indication with --stdin" {
  run "$EXEC" -r 3 -t pandemy -a .XH --stdin < etat.txt
  [ "$status" -eq 8 ]
  [ "${lines[0]}" = "Error: The number of rows and columns cannot be indicated together with the option --stdin" ]

}

@test "Wrong cell state in stdinn" {
  run "$EXEC" -t pandemy -a .bH --stdin < etat.txt
  [ "$status" -eq 9 ]

}

@test "Wrong row/col lenghts of the initial state in stdin" {
  run "$EXEC" -t pandemy -a .XH --stdin < etat2.txt
  [ "$status" -eq 10 ]
  [ "${lines[0]}" = "Error: All rows and columns should be of the same length" ]

  
}