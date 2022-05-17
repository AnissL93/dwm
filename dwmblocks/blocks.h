// Modify this file to change what commands output to your statusbar, and
// recompile using the make command.

static const Block blocks[] = {
    /*Icon*/ /*Command*/ /*Update Interval*/ /*Update Signal*/
    // weather, update every 8 hours
    {"", "show_weather", 2 * 60 * 60 * 8, 1},

    // network
    {"", "show_network", 2, 5},

    // show status of input method
    {"", "input_method", 1, 2},

    // memory
    {"💾 ", "show_resource", 10, 3},

    // battery
    {"", "battery", 6, 4},

    // date
    {"", "d=$(date '+%b %d (%a) %I:%M%p'); echo $d", 120, 0},
};

// sets delimeter between status commands. NULL character ('\0') means no
// delimeter.
static char delim[] = " | ";
static unsigned int delimLen = 5;
