export LC_ALL=en_US.UTF-8
export PATH=/home/namanj/bin:/bin:/sbin:/usr/bin:/usr/sbin:/usr/X11R7/bin:/usr/pkg/bin:/usr/pkg/sbin:/usr/games:/usr/local/bin:/usr/local/sbin:/home/namanj/.local/bin

alias 'code'='cd /usr/tests/lib/libcurses'
alias 'test'='cd /home/namanj/progs'
alias noverify='git config --global http.sslVerify false'
alias verify='git config --global http.sslVerify true'

export CURSES_TRACE_MASK=0x080
export CURSES_TRACE_FILE=./logs

# Git shortcuts
alias gl='git log --oneline'
alias gb='git branch'
alias gc='git commit -m'
alias gs='git status'
alias gd='git diff'
