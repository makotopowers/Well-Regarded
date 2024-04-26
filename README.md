# Well-Regarded

We shall solve the Regarded game. 

## Current Status
Finishing game logic. 

## Next 
- Add a way to efficiently store the game state.
- Once this is done, need to figure out exactly what the learning goal is. 
    - Current idea: assign a value to each state based on how many cards we must play to reach a winning position, how many cards the opponent needs to play to 
    retake the winning position, with fixed variables at the start of each round of tricks won, jokers remaining, and cards played. 


## To install

```bash
git clone git@github.com:makotopowers/Well-Regarded.git
cd Well-Regarded
```



## To run 

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Regarded
```

## To test

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest
```

