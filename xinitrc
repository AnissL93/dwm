eval "$(dbus-launch --sh-syntax --exit-with-session)"
export LANG=zh_CN.UTF-8
export LC_ALL="en_US.UTF-8"
export XMODIFIERS="@im=fcitx"
export QT_IM_MODULE="fcitx"
export GTK_IM_MODULE="fcitx"
exec fcitx &gt; /dev/null &amp;

redshift &

xmodmap ~/.config/x11/xmodmap

~/.config/x2ray/bin/xray run --config ~/.config/x2ray/vless_tcp_xtls.json &

xbacklight -set $(cat ~/.config/X11/xbacklight)

~/.config/Scripts/selmon init

xcompmgr &

xwallpaper --zoom "$HOME/.local/share/bg"
wal -i $(realpath "$HOME/.local/share/bg") --backend colorz
xrdb -merge ~/.Xresources

xset -dpms
xset s off

emacs --daemon
dwmblocks &
exec dwm

