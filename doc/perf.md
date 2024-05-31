# 使用 perf 抓取火焰图

1. 脚本

```shell
#!/usr/bin/env bash

set -e

function usage() {
    echo "Usage:"
    echo "  $(basename $0) [-F req] [-p pid]"
    echo "  $(basename $0) [-F req] [binary]"
    echo "注意: -F 默认是 /proc/sys/kernel/perf_event_max_sample_rate 的值"
}

FlameGraph_home="${HOME}/go/src/github.com/brendangregg/FlameGraph"

if [ -z "$1" ]; then
    usage
    exit 1
fi

if [ ! -d "$FlameGraph_home" ]; then
    mkdir -p "${HOME}/go/src/github.com/brendangregg"
    git clone --depth 1 https://github.com/brendangregg/FlameGraph "${HOME}/go/src/github.com/brendangregg/FlameGraph"
fi

output_file=$(date '+%Y%m%d%H%M%S')
process_output_perf="output/${output_file}.data"
process_output_svg="output/${output_file}.svg"
function parse_perf() {
    echo "proccess done...."
    perf script -i "$process_output_perf" | "$FlameGraph_home"/stackcollapse-perf.pl | "$FlameGraph_home"/flamegraph.pl >"${process_output_svg}"
    echo "open ${process_output_svg}"
}

# 移除文件
rm -rf "$process_output_perf"
rm -rf "$process_output_svg"

mkdir -p output

# 结束的时候
trap parse_perf EXIT

set -x

perf record -o "$process_output_perf" -g -F 100 "$@"
```

2. 使用

- 开启抓取

```shell
./perf.sh -F 10000 -p <PID>
```

- 关闭抓取: ctrl + c
