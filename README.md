# 🖥️ Unix Shell (msh)
<img width="300" height="300" alt="unix-shell" src="https://github.com/user-attachments/assets/2a4314a4-0959-427b-9c46-3894b4e65491" />

A custom Unix shell written in C that supports interactive and batch modes, built-in commands, process creation, and I/O redirection. This project demonstrates systems-level programming, process management, and command-line interface design using fundamental Unix system calls (fork, execv, wait, etc.).

# 📖 About the Project
The Unix Shell (msh) is a simplified version of popular shells like bash. It provides both an interactive prompt and a batch mode for executing commands from a file.

## This project was designed to:

- ⚡ Gain experience in Linux/Unix programming environments

- 🔧 Understand how processes are created, executed, and managed

- 🖥️ Implement the core functionality of shells (prompt, parsing, execution, redirection)

## Key highlights:

- Parses and executes user commands using fork() and execv()

- Handles built-in commands (exit, quit, cd)

- Supports output redirection (> to overwrite files)

- Provides robust error handling with a single consistent error message

- Searches commands in /bin, /usr/bin, /usr/local/bin, and current directory (./)


# 🚀 Features

- 🖱️ Interactive Mode → ./msh launches a prompt (msh>)

- 📂 Batch Mode → ./msh batch.txt executes commands from a file

- ⚡ Built-in Commands:

                       - exit → exits shell

                       - quit → exits shell

                       - cd <dir> → changes working directory

- 📝 Command Execution → runs any executable in /bin, /usr/bin, /usr/local/bin, or ./

- 🔄 I/O Redirection: ls -la > output.txt (redirects both stdout and stderr)

- 🐞 Error Handling: consistent error output: 

         char error_message[30] = "An error has occurred\n";
         write(STDERR_FILENO, error_message, strlen(error_message));


- 📊 Supports up to 10 command-line parameters per command

# 🛠️ Tech Stack
- Language: C

- System Calls: fork, execv, wait, chdir, access, write

- Build: GNU Make

- Testing: Provided test-msh.sh test harness

# ▶️ Building and Running

## 🔨 Build
make

## 🖱️ Interactive Mode
./msh
msh> ls -la
msh> cd ..
msh> quit

## 📂 Batch Mode
./msh batch.txt

# 📊 Example Usage
Interactive: 

msh> ls -l /tmp
(total output of ls...)
msh> cd /usr/local
msh> pwd
/usr/local
msh> exit

Batch File (batch.txt):

ls
pwd
cd /bin
ls -la > bin_output.txt

# 🎯 Learning Outcomes
Through this project, I gained hands-on experience in:

- Implementing a command-line interpreter in C

- Mastering process creation and management with fork, exec, wait

- Handling I/O redirection with file descriptors

- Writing robust error handling and enforcing consistent outputs

- Designing software under strict constraints (no system(), limited arguments, consistent formatting)


# 🚀 Future Enhancements

- 🔀 Add pipes (|) to chain commands

- 🧵 Add background process execution (&)

- 🔎 Add PATH environment variable parsing

- ⏱️ Add command history and tab autocompletion


# ✨ 
This project showcases low-level systems programming skills, process management expertise, and C development proficiency — all highly valuable for operating systems, backend, and systems software roles.





                       

