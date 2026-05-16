#!/usr/bin/env python3
# 从 homework/experiment_*.c 生成 _posts/*.md
# 用法：在仓库根目录运行 `python tools/generate_posts.py`
# 幂等：每次都会先清空 _posts/ 下以 "experiment-" 命名的旧文件再重写

import re
import subprocess
from datetime import datetime, timedelta, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HOMEWORK = ROOT / "homework"
POSTS = ROOT / "_posts"

CST = timezone(timedelta(hours=8))

EXPERIMENT_META = {
    1: {
        "slug": "sequential-list-books",
        "title": "实验 1 · 顺序表 · 图书管理",
        "tags": ["C", "顺序表", "线性表"],
        "summary": "顺序表的基础 CRUD：用连续数组保存图书记录，演示按位置/按书号查找、插入、删除。",
        "files": ["experiment_1.c"],
    },
    2: {
        "slug": "sequential-list-menu",
        "title": "实验 2 · 顺序表 · 菜单系统",
        "tags": ["C", "顺序表", "菜单"],
        "summary": "在实验 1 的顺序表上加菜单驱动入口、输入校验、容量检查，把零散操作整合成一个可交互的小系统。",
        "files": ["experiment_2.c"],
    },
    3: {
        "slug": "linked-list-books",
        "title": "实验 3 · 单链表 · 图书",
        "tags": ["C", "链表", "线性表"],
        "summary": "把实验 1 的顺序表换成带头/尾指针的单链表实现，体会节点分配、链式遍历、头插与尾插的差异。",
        "files": ["experiment_3.c"],
    },
    4: {
        "slug": "linked-list-bignum",
        "title": "实验 4 · 大数链表",
        "tags": ["C", "链表", "大整数"],
        "summary": "用单链表保存任意位长的整数：每个节点存一位，演示字符串到链表的尾插构造，便于做加法/比较等大数运算。",
        "files": ["experiment_4.c"],
    },
    5: {
        "slug": "linked-stack",
        "title": "实验 5 · 链栈 · 字符",
        "tags": ["C", "栈", "链表"],
        "summary": "链式栈：每个节点存一个字符，无容量上限，入栈动态分配、出栈释放节点。",
        "files": ["experiment_5.c"],
    },
    6: {
        "slug": "infix-to-postfix",
        "title": "实验 6 · 中缀转后缀 + 求值",
        "tags": ["C", "栈", "表达式"],
        "summary": "双栈实现表达式处理：一个栈处理运算符优先级把中缀转为后缀，另一个数字栈完成最终求值。",
        "files": ["experiment_6.c"],
    },
    7: {
        "slug": "linked-queue-hospital",
        "title": "实验 7 · 链队列 · 医院叫号",
        "tags": ["C", "队列", "模拟"],
        "summary": "链式队列模拟医院叫号：按到达时间入队、按治疗时间出队，统计平均等待与平均处理时间。",
        "files": ["experiment_7.c"],
    },
    8: {
        "slug": "round-robin-scheduling",
        "title": "实验 8 · 时间片轮转调度",
        "tags": ["C", "队列", "操作系统"],
        "summary": "时间片轮转作业调度：作业按到达时间入队，每次取一个时间片执行；没做完就重新排到队尾，最终统计平均周转/等待时间。",
        "files": ["experiment_8.c"],
    },
    9: {
        "slug": "string-cipher",
        "title": "实验 9 · 字符串加解密",
        "tags": ["C", "字符串", "映射表"],
        "summary": "用 26 字母固定置换表做加密；解密表在启动时一次性反推出来，避免每次解密都重新扫描映射表。",
        "files": ["experiment_9.c"],
    },
    10: {
        "slug": "string-ops",
        "title": "实验 10 · 字符串操作系统",
        "tags": ["C", "字符串", "命令解析"],
        "summary": "字符串操作演示系统：用串头表（串名/串首并行数组）管理多个串，支持按命令名解析、参数分发与多串协同操作。",
        "files": ["experiment_10.c"],
    },
    11: {
        "slug": "sparse-matrix-triplet",
        "title": "实验 11 · 稀疏矩阵 · 三元组表",
        "tags": ["C", "矩阵", "三元组"],
        "summary": "用 (行, 列, 值) 三元组紧凑表示稀疏矩阵，按行优先校验录入；位置编码 `i*n+j` 用 `long` 显式防溢出。",
        "files": ["experiment_11.c"],
    },
    12: {
        "slug": "generalized-list-library",
        "title": "实验 12 · 广义表算法库 (多文件)",
        "tags": ["C", "广义表", "多文件"],
        "summary": "广义表的递归算法库：单独抽出 `glist.h` / `glist.c`，演示带头节点的广义表构造、嵌套解析、长度与深度计算。",
        "files": [
            "experiment_12/main.c",
            "experiment_12/glist.c",
            "experiment_12/glist.h",
        ],
    },
    13: {
        "slug": "generalized-list-ops",
        "title": "实验 13 · 广义表 · 复制 / 长度 / 深度",
        "tags": ["C", "广义表", "递归"],
        "summary": "单文件版广义表实验：互相调用的解析器递归吃完嵌套括号，再用同样递归思路求复制、长度、深度。配套有自动化测试程序。",
        "files": ["experiment_13.c"],
    },
    14: {
        "slug": "sparse-matrix-saddle-point",
        "title": "实验 14 · 稀疏矩阵 · 求鞍点",
        "tags": ["C", "矩阵", "三元组"],
        "summary": "在三元组表稀疏矩阵上找「鞍点」：该位置既是所在行的最大值，又是所在列的最小值。",
        "files": ["experiment_14.c"],
    },
    16: {
        "slug": "binary-tree-basics",
        "title": "实验 16 · 二叉树 · 基本操作",
        "tags": ["C", "二叉树", "遍历", "队列"],
        "summary": "二叉链表 + 顺序栈 + 链队列三件套：先/中/后序遍历（递归 + 非递归），层序遍历，最大宽度（队列节点附带 level 字段）。",
        "files": ["experiment_16.c"],
    },
}

CATEGORIES = ["数据结构", "实验"]


def get_first_commit_date(rel_path: str) -> datetime | None:
    # 取文件第一次被加入 git 的提交时间，作为博文日期
    try:
        out = subprocess.run(
            [
                "git",
                "-C",
                str(ROOT),
                "log",
                "--diff-filter=A",
                "--format=%aI",
                "--",
                rel_path,
            ],
            capture_output=True,
            text=True,
            check=False,
        )
    except FileNotFoundError:
        return None
    lines = [line.strip() for line in out.stdout.splitlines() if line.strip()]
    if not lines:
        return None
    return datetime.fromisoformat(lines[-1])


def language_for(path: Path) -> str:
    return "c" if path.suffix in {".c", ".h"} else "text"


def build_post(exp_num: int, meta: dict, post_date: datetime) -> str:
    tags_yaml = ", ".join(f'"{t}"' for t in meta["tags"])
    cats_yaml = ", ".join(f'"{c}"' for c in CATEGORIES)
    date_str = post_date.strftime("%Y-%m-%d %H:%M:%S %z")

    parts = [
        "---",
        f'title: "{meta["title"]}"',
        f"date: {date_str}",
        f"categories: [{cats_yaml}]",
        f"tags: [{tags_yaml}]",
        "math: false",
        "pin: false",
        "toc: true",
        "comments: false",
        "---",
        "",
        meta["summary"],
        "",
        "> 课程：《数据结构》（涂老师）。本文代码遵循仓库 [`AGENTS.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/AGENTS.md) 与 [`CLAUDE.md`](https://github.com/modestteng/Data-Structures-Course/blob/main/CLAUDE.md) 的约定。",
        "",
        "## 源代码",
        "",
    ]

    multi_file = len(meta["files"]) > 1
    for rel_file in meta["files"]:
        full_path = HOMEWORK / rel_file
        if not full_path.exists():
            parts.append(f"> *源文件 `{rel_file}` 缺失，跳过。*\n")
            continue
        code = full_path.read_text(encoding="utf-8", errors="replace")
        lang = language_for(full_path)
        if multi_file:
            display_name = Path(rel_file).name
            parts.append(f"### `{display_name}`")
            parts.append("")
        parts.append(f"```{lang}")
        parts.append(code.rstrip())
        parts.append("```")
        parts.append("")

    parts.append("## 运行示例")
    parts.append("")
    parts.append(
        "在 VS Code 打开源文件后按 **Ctrl+Shift+B** 运行项目预置的 `Run current C file` 任务，"
        "或在命令行：\n\n"
        "```powershell\n"
        f"gcc -finput-charset=UTF-8 -fexec-charset=UTF-8 homework/{meta['files'][0]} -o homework/experiment_{exp_num}.exe\n"
        f".\\homework\\experiment_{exp_num}.exe\n"
        "```\n"
    )

    return "\n".join(parts) + "\n"


def slugify(name: str) -> str:
    return re.sub(r"[^a-z0-9-]", "", name.lower())


def main() -> int:
    POSTS.mkdir(exist_ok=True)
    # 幂等：先清掉旧的自动生成博文
    for old in POSTS.glob("*-experiment-*.md"):
        old.unlink()

    fallback_base = datetime(2026, 3, 1, 9, 0, 0, tzinfo=CST)
    written = 0
    used_dates: dict[str, int] = {}

    for exp_num in sorted(EXPERIMENT_META):
        meta = EXPERIMENT_META[exp_num]
        primary_file = meta["files"][0]
        rel_in_repo = f"homework/{primary_file}"
        commit_date = get_first_commit_date(rel_in_repo)
        if commit_date is None:
            post_date = fallback_base + timedelta(days=exp_num * 3)
        else:
            # 同一天 commit 的实验按实验号顺延一分钟，保证排序稳定
            base = commit_date.astimezone(CST).replace(
                hour=9, minute=0, second=0, microsecond=0
            )
            post_date = base + timedelta(minutes=exp_num)

        day_key = post_date.strftime("%Y-%m-%d")
        used_dates[day_key] = used_dates.get(day_key, 0) + 1

        slug = slugify(meta["slug"])
        out_path = POSTS / f"{day_key}-experiment-{exp_num}-{slug}.md"
        out_path.write_text(build_post(exp_num, meta, post_date), encoding="utf-8")
        written += 1
        print(f"  generated {out_path.name}")

    print(f"\n[OK] {written} posts written to {POSTS}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
