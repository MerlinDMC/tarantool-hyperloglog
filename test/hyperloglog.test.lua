#!/usr/bin/env tarantool

package.path = "../?.lua;../?/init.lua;./?.lua;./?/init.lua"
package.cpath = "../?.so;../?.dylib;./?.so;./?.dylib"

local hyperloglog = require('hyperloglog')

require('tap').test('hyperloglog', function(test)
    test:plan(5)

    local hll = hyperloglog.new()
    test:is(hll:count(), 0, "Empty HLL return zero cardinality")

    hll:add("key1")
    test:is(hll:count(), 1, "Single key has been added correctly")

    hll:add("key1")
    test:is(hll:count(), 1, "Duplicate keys do not increment the cardinality")

    hll:add("key2")
    test:is(hll:count(), 2, "Additional keys do increment the cardinality")

    for i=100000,0,-1 do
      hll:add("foo" .. i)
    end
    test:ok(100000/hll:count() > 0.94, "accuracy is at expected level")
end)
