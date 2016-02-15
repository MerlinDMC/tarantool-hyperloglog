package = 'hyperloglog'
version = 'scm-1'

source  = {
    url    = 'git://github.com/MerlinDMC/tarantool-hyperloglog.git';
    branch = 'master';
}

description = {
    summary  = "A hyperloglog module for tarantool";
    detailed = [[
    A native C extension for tarantool that computes cardinalities
    using the hyperloglog algorithm.
    ]];
    homepage = 'https://github.com/MerlinDMC/tarantool-hyperloglog.git';
    license  = 'BSD';
    maintainer = "Daniel Malon <daniel.malon@me.com>";
}

dependencies = {
    'lua >= 5.1';
}

external_dependencies = {
    TARANTOOL = {
        header = 'tarantool/module.h'
    };
}

build = {
    type = 'builtin',

    modules = {
        ['hyperloglog'] = {
            incdirs = {
                '$(TARANTOOL_INCDIR)/tarantool'
            };
            sources = {
                'hyperloglog/lua_hyperloglog.c',
                'hyperloglog/redis_hyperloglog.c'
            };
        }
    }
}
-- vim: syntax=lua ts=4 sts=4 sw=4 et
