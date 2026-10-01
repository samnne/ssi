# Simple Shell Interpreter

## What it does?

`ssi.c` is a simple shell interpreter that runs basic kernal commands and any program available in your 
PATH. The program uses system functions for change directory functionality, maintains command history with the 
'history' command as well as other different features of a shell.

## Extra Features not required

### `history`: gets command history written in the p1 shell

The history of a current session gets appended to db/history.txt and persists past sessions.

Example Usage
```bash
username@hostname: /home/username/ > history
touch main.c
cd build
history
```

### UI Prompt

The color of the prompt ui changed from a basic default grey into a blue to distinguish between a user prompt and the 
bash prompt. 

### Supports certain git functions
 
The shell also supports some git functionality. The prompt displays (null) if it doesn't have a git repo in the current folder, and displays 'branch-name' if there exists a git repo.
```bash
username@hostname: /home/username/ * branch-name > 
```

The feature has its limits as this implementation uses strtok() to tokenize a users input, which makes it increasingly difficult to capture a nested quotes i.e `git commit -m "foo bar"` will break git in this ssi, but `git commit -m "foobar" will work as the tokenizer splits across spaces. 

## Future improvements 

Keyboard Handling: Cursor movement and handling keyboard input. For example using arrow keys to move the input cursor to insert between prompts. 

Arrow Key History: This extends the above improvement, but in all shell implemtations, pressing the up arrow key or the down arrow key, it traverses through the current shell history db. 




