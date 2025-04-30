#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

// file_descriptor: 0-> read, 1-> write, 2-> error

vector<string> tokenizeCommand(string str, char delim) {
    vector<string> strTokens;
    string tmp = "";

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            strTokens.push_back(tmp);
            tmp = "";
        } else if (str[i] == '&') {
            if (!tmp.empty()) strTokens.push_back(tmp);
            strTokens.push_back("bgRunTriggered");
            return strTokens;
        } else if (str[i] == '\'') {
            // ignore quotes
        } else {
            tmp += str[i];
        }
    }
    if (!tmp.empty()) strTokens.push_back(tmp);
    return strTokens;
}

vector<string> separateCommands(string str, char delim) {
    vector<string> strTokens;
    string tmp = "";
    bool foundPipe = false;

    for (int i = 0; i < str.length(); i++) {
        if (foundPipe) {
            foundPipe = false;
            continue;
        }

        if (str[i] == delim) {
            strTokens.push_back(tmp);
            tmp = "";
            foundPipe = true;
        } else if (str[i] == ' ' && i + 1 < str.length() && str[i + 1] == delim) {
            // skip
        } else {
            tmp += str[i];
        }
    }
    if (!tmp.empty()) strTokens.push_back(tmp);
    return strTokens;
}

void shiftAndPop(vector<string>& vec) {
    if (vec.empty()) return;
    for (size_t i = 0; i < vec.size() - 1; ++i)
        vec[i] = vec[i + 1];
    vec.pop_back();
}

int main() {
    string cmd;
    pid_t ret_Val;
    int childCount = 0;
    bool pipesFlg = false;
    vector<string> history;
    int historyCommandsCounter = 0;

    while (true) {
        cout << "\n( Enter Command ) : ";
        getline(cin, cmd);
        if (cmd == "exit")
            break;

        if (cmd == "history") {
            int srNo = history.size();
            cout << "\n( Total Commands entered: " << historyCommandsCounter << " )\n";
            cout << "( Total history size: 10 )\n";
            cout << "( Current history size: " << history.size() << " )\n";

            cout << "\nShowing HISTORY...\n";
            for (int i = history.size() - 1; i >= 0; i--) {
                cout << srNo-- << "-> " << history[i] << endl;
            }
            continue;
        }

        if (cmd == "!!") {
            if (history.empty()) {
                cout << "history is empty\n";
                continue;
            }
            cmd = history.back();
            cout << "most recent command was " << cmd << endl << endl;
        }

        if (cmd[0] == '!' && cmd[1] != '!') {
            string numStr = cmd.substr(1);
            int no = stoi(numStr);

            if (no > history.size() || no == 0) {
                cout << " command - " << no << " not found in history! (//ERROR_404)\n";
                continue;
            }
            cout << "no " << no << " command will be going to execute...\n\n";
            cmd = history[no - 1];
        }

        historyCommandsCounter++;
        if (historyCommandsCounter > 10) {
            shiftAndPop(history);
        }
        history.push_back(cmd);

        vector<string> commands = separateCommands(cmd, '|');
        if (commands.size() > 1)
            pipesFlg = true;

        int prev_pipe[2], next_pipe[2];

        for (size_t i = 0; i < commands.size(); i++) {
            vector<string> tokens = tokenizeCommand(commands[i], ' ');

            bool bgRunFlag = false;
            if (!tokens.empty() && tokens.back() == "bgRunTriggered") {
                bgRunFlag = true;
                tokens.pop_back();
            }

            if (tokens.empty()) continue;

            if (tokens[0] == "cd") {
                if (tokens.size() < 2) {
                    cout << "Usage: cd <directory>" << endl;
                } else if (chdir(tokens[1].c_str()) != 0) {
                    perror("chdir failed");
                }
                continue;
            }

            if (tokens[0] == "mkfifo") {
                if (tokens.size() < 2) {
                    cout << "Usage: mkfifo <filename> " << endl;
                } else if (mkfifo(tokens[1].c_str(), 0666) != 0) {
                    perror("mkfifo failed!\n");
                } else {
                    cout << "named pipe : " << tokens[1] << " created successfully!\n";
                }
                continue;
            }

            if (i < commands.size() - 1 && pipesFlg && pipe(next_pipe) == -1) {
                perror("pipe creation failed!\n");
                exit(EXIT_FAILURE);
            }

            childCount++;
            ret_Val = fork();

            if (ret_Val < 0) {
                cerr << "\nFork() failed!\n";
            } else if (ret_Val == 0) {
                cout << "\nchild: " << getpid() << "\n";

                for (size_t i = 0; i < tokens.size(); i++) {
                    if (tokens[i] == "<" && i + 1 < tokens.size()) {
                        int fd_read = open(tokens[i + 1].c_str(), O_RDONLY);
                        dup2(fd_read, 0);
                        close(fd_read);
                        tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
                        break;
                    }
                }

                for (size_t i = 0; i < tokens.size(); i++) {
                    if (tokens[i] == ">" && i + 1 < tokens.size()) {
                        int fd_write = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        dup2(fd_write, 1);
                        close(fd_write);
                        tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
                        break;
                    }
                }

                for (size_t i = 0; i < tokens.size(); i++) {
                    if (tokens[i] == "2>" && i + 1 < tokens.size()) {
                        int fd_error = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                        dup2(fd_error, 2);
                        close(fd_error);
                        tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
                        break;
                    }
                }

                if (pipesFlg) {
                    if (i < commands.size() - 1)
                        dup2(next_pipe[1], STDOUT_FILENO);
                    if (i > 0)
                        dup2(prev_pipe[0], STDIN_FILENO);

                    close(prev_pipe[0]);
                    close(prev_pipe[1]);
                    close(next_pipe[0]);
                    close(next_pipe[1]);
                }

                char* args[tokens.size() + 1];
                for (size_t j = 0; j < tokens.size(); j++)
                    args[j] = const_cast<char*>(tokens[j].c_str());
                args[tokens.size()] = NULL;

                if (execvp(args[0], args) == -1) {
                    cout << "\nCOMMAND NOT IDENTIFIED BY SHELL!\n";
                    exit(EXIT_FAILURE);
                }
            } else {
                cout << "\n_____________________________________\n";
                cout << "\nparent: " << getpid() << "\n";

                if (pipesFlg) {
                    if (i < commands.size() - 1)
                        close(next_pipe[1]);
                    if (i > 0)
                        close(prev_pipe[0]);

                    prev_pipe[0] = next_pipe[0];
                    prev_pipe[1] = next_pipe[1];
                }

                if (!bgRunFlag) {
                    while (childCount > 0) {
                        int status;
                        pid_t c_pid = wait(&status);
                        if (status == 0)
                            cout << "child with pid " << c_pid << " terminated!\n";
                        childCount--;
                    }
                } else {
                    cout << "\nchild running in background!\n";
                }
            }
        }
    }

    cout << "\n***** shell terminated! *****\n";
    return 0;
}
