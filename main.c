#include "types.h"

#include <stz/args.h>

struct
{
    Str0 path;
    bool help;
} cfg = {
    .path = Str0_("./"),
    .help = false,
};

// clang-format off
Arg options[] = {
    {Str_("help"), no_argument,       0, Str_("Show help"),  ARG_Bool,   &cfg.help, false},
    {Str_("path"), required_argument, 0, Str_("Input path"), ARG_String, &cfg.path, false},
};
// clang-format on

int main(int argc, char* argv[])
{

    int   err  = 0;
    Arena perm = arena_new(MB_);

    Args args = {.buf = options, .len = countof(options), .usage = Str0_("xjson [OPTIONS]")};

    err = args_parse(&perm, &args, argc, argv, true);
    OnError_Goto(err, __close, "Failed: args_parse(err=%d)", err);

    if (cfg.help)
    {
        args_dump_help(&args);
        return EXIT_SUCCESS;
    }

__close:
    arena_free(&perm);

    return err ? EXIT_SUCCESS : EXIT_FAILURE;
}
