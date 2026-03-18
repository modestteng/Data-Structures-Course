import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, Polygon, PathPatch
from matplotlib.path import Path
from matplotlib import font_manager as fm


# =========================================
# 1. 鑷姩瀵绘壘涓枃瀛椾綋锛屽敖閲忛伩鍏嶄贡鐮?
# =========================================
def get_chinese_font():
    candidates = [
        "Microsoft YaHei",
        "SimHei",
        "SimSun",
        "KaiTi",
        "Noto Sans CJK SC",
        "Source Han Sans SC",
        "WenQuanYi Zen Hei",
        "PingFang SC",
    ]

    for name in candidates:
        try:
            path = fm.findfont(name, fallback_to_default=False)
            if path and os.path.exists(path):
                return fm.FontProperties(fname=path)
        except Exception:
            pass

    common_paths = [
        r"C:\Windows\Fonts\msyh.ttc",
        r"C:\Windows\Fonts\simhei.ttf",
        r"C:\Windows\Fonts\simsun.ttc",
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc",
        "/System/Library/Fonts/PingFang.ttc",
    ]

    for p in common_paths:
        if os.path.exists(p):
            return fm.FontProperties(fname=p)

    return None


FONT = get_chinese_font()
plt.rcParams["axes.unicode_minus"] = False


# =========================================
# 2. 涓€浜涢€氱敤缁樺浘鍑芥暟
# =========================================
def add_round_box(ax, x, y, w, h,
                  facecolor="#dbe3fb",
                  edgecolor="#6b83c4",
                  linewidth=1.8,
                  radius=0.04,
                  alpha=1.0,
                  zorder=2):
    box = FancyBboxPatch(
        (x, y), w, h,
        boxstyle=f"round,pad=0.008,rounding_size={radius}",
        linewidth=linewidth,
        edgecolor=edgecolor,
        facecolor=facecolor,
        alpha=alpha,
        zorder=zorder
    )
    ax.add_patch(box)
    return box


def add_rect_box(ax, x, y, w, h,
                 facecolor="#dbe3fb",
                 edgecolor="#6b83c4",
                 linewidth=1.6,
                 alpha=1.0,
                 zorder=2):
    rect = plt.Rectangle(
        (x, y), w, h,
        facecolor=facecolor,
        edgecolor=edgecolor,
        linewidth=linewidth,
        alpha=alpha,
        zorder=zorder
    )
    ax.add_patch(rect)
    return rect


def add_corner_fold(ax, x, y, w, h, size=0.07, color="#f4f4f4", zorder=4):
    tri = Polygon(
        [[x + w - size, y + h], [x + w, y + h], [x + w, y + h - size]],
        closed=True,
        facecolor=color,
        edgecolor="none",
        zorder=zorder
    )
    ax.add_patch(tri)


def add_vertical_gradient(ax, x, y, w, h,
                          top_color=(0.88, 0.93, 1.0),
                          mid_color=(0.42, 0.58, 0.95),
                          bottom_color=(0.88, 0.93, 1.0),
                          zorder=1):
    n = 600
    gradient = np.zeros((n, 2, 3))

    for i in range(n):
        t = i / (n - 1)
        if t < 0.5:
            tt = t / 0.5
            c = np.array(top_color) * (1 - tt) + np.array(mid_color) * tt
        else:
            tt = (t - 0.5) / 0.5
            c = np.array(mid_color) * (1 - tt) + np.array(bottom_color) * tt
        gradient[i, :, :] = c

    ax.imshow(
        gradient,
        extent=(x, x + w, y, y + h),
        origin="lower",
        aspect="auto",
        zorder=zorder
    )


def draw_bracket(ax, x, y_top, y_bottom, width=0.035, color="#233f91", lw=6):
    """
    鐢诲乏渚уぇ鎷彿鏍峰紡杩炴帴绾?
    """
    mid = (y_top + y_bottom) / 2

    verts = [
        (x + width, y_top),         # 璧风偣
        (x, y_top),                 # 鎺у埗
        (x, y_top),                 # 鎺у埗
        (x, mid + 0.02),            # 缁撴潫

        (x, mid + 0.005),           # 缁х画
        (x, mid),                   # 鎺у埗
        (x + width * 0.6, mid),     # 鎺у埗
        (x + width, mid),           # 缁撴潫

        (x + width * 0.6, mid),     # 鏂版
        (x, mid),                   # 鎺у埗
        (x, mid - 0.005),           # 鎺у埗
        (x, y_bottom + 0.02),       # 缁撴潫

        (x, y_bottom),              # 鎺у埗
        (x, y_bottom),              # 鎺у埗
        (x + width, y_bottom),      # 缁撴潫
        # // Block: 补齐路径顶点，保证 Path 的 vertices 与 codes 数量一致（点与指令一一对应）
        (x + width, y_bottom),      # 与最后一组 CURVE4 对齐的终点
    ]

    codes = [
        Path.MOVETO,
        Path.CURVE4, Path.CURVE4, Path.CURVE4,

        Path.MOVETO,
        Path.CURVE4, Path.CURVE4, Path.CURVE4,

        Path.MOVETO,
        Path.CURVE4, Path.CURVE4, Path.CURVE4,

        Path.MOVETO,
        Path.CURVE4, Path.CURVE4, Path.CURVE4,
    ]

    path = Path(verts, codes)
    patch = PathPatch(
        path,
        facecolor="none",
        edgecolor=color,
        lw=lw,
        capstyle="round",
        joinstyle="round",
        zorder=3
    )
    ax.add_patch(patch)


# =========================================
# 3. 涓荤粯鍥?
# =========================================
def draw_panel():
    fig, ax = plt.subplots(figsize=(14, 8), dpi=160)
    fig.patch.set_facecolor("#eef1f7")
    ax.set_facecolor("#eef1f7")
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.axis("off")

    # ---------- 宸︿晶钃濊壊绔栨潯 ----------
    left_x, left_y, left_w, left_h = 0.06, 0.18, 0.075, 0.55

    # 鍙戝厜搴曞眰
    add_round_box(
        ax, left_x - 0.006, left_y - 0.006, left_w + 0.012, left_h + 0.012,
        facecolor="#7da2ff", edgecolor="none", alpha=0.18, radius=0.018, zorder=0
    )

    # 涓讳綋鎻忚竟
    add_round_box(
        ax, left_x, left_y, left_w, left_h,
        facecolor="none", edgecolor="#2e5de0", linewidth=1.8, radius=0.015, zorder=3
    )

    # 娓愬彉濉厖
    add_vertical_gradient(
        ax,
        left_x + 0.002, left_y + 0.002,
        left_w - 0.004, left_h - 0.004,
        top_color=(0.93, 0.96, 1.0),
        mid_color=(0.36, 0.53, 0.94),
        bottom_color=(0.93, 0.96, 1.0),
        zorder=1
    )

    # 鍐呴儴楂樺厜绾?
    ax.plot(
        [left_x + left_w * 0.15, left_x + left_w * 0.85],
        [left_y + left_h * 0.97, left_y + left_h * 0.97],
        color="white", linewidth=1.2, alpha=0.8, zorder=4
    )
    ax.plot(
        [left_x + left_w * 0.15, left_x + left_w * 0.85],
        [left_y + left_h * 0.03, left_y + left_h * 0.03],
        color="white", linewidth=1.0, alpha=0.55, zorder=4
    )

    # 鏂囧瓧锛堢珫鎺掞級
    left_text = "智\n能\n停\n车\n管\n理\n系\n统"
    ax.text(
        left_x + left_w / 2,
        left_y + left_h / 2,
        left_text,
        ha="center",
        va="center",
        fontsize=23,
        color="#111111",
        fontproperties=FONT,
        zorder=5
    )

    # ---------- 涓棿鎷彿 ----------
    draw_bracket(ax, x=0.19, y_top=0.79, y_bottom=0.13, width=0.04, color="#223b88", lw=5)

    # ---------- 鍙充笂淇℃伅妗?----------
    box1_x, box1_y, box1_w, box1_h = 0.27, 0.53, 0.58, 0.33
    add_rect_box(ax, box1_x, box1_y, box1_w, box1_h,
                 facecolor="#d7def6", edgecolor="#6a7fba", linewidth=1.3, zorder=2)
    add_corner_fold(ax, box1_x, box1_y, box1_w, box1_h, size=0.06)

    ax.text(
        box1_x + 0.02, box1_y + box1_h - 0.07,
        "智能停车位管理",
        fontsize=20, fontweight="bold",
        color="#1f2850",
        fontproperties=FONT,
        zorder=5
    )

    text1 = (
        "鏅鸿兘鍋滆溅浣嶇鐞嗗姛鑳介潰鍚戜笟涓诲拰绠＄悊浜哄憳銆傚畨瑁呮櫤鑳借溅浣嶉攣鍚庯紝閫氳繃\n"
        "瀹㈡埛绔敞鍐岃处鎴枫€佸～鍐欏仠杞︿綅淇℃伅骞惰幏鍙栫鐞嗘潈闄愶紝鐩戞祴鍜岀鐞嗚溅浣嶃€俓n"
        "涓氫富鎴栫墿涓氬彲璁惧畾鍏变韩鏃舵锛岀敤鎴峰彲鏌ヨ銆侀绾﹀強浣跨敤杞︿綅銆傝溅浣嶈\n"
        "预约后，客户端更新状态为有车，其他用户无法再预约。"
    )
    ax.text(
        box1_x + 0.015, box1_y + box1_h - 0.10,
        text1,
        fontsize=15.5,
        color="#111111",
        linespacing=1.7,
        va="top",
        fontproperties=FONT,
        zorder=5
    )

    # ---------- 鍙充笅淇℃伅妗?----------
    box2_x, box2_y, box2_w, box2_h = 0.27, 0.13, 0.58, 0.28
    add_rect_box(ax, box2_x, box2_y, box2_w, box2_h,
                 facecolor="#d7def6", edgecolor="#6a7fba", linewidth=1.3, zorder=2)
    add_corner_fold(ax, box2_x, box2_y, box2_w, box2_h, size=0.06)

    ax.text(
        box2_x + 0.02, box2_y + box2_h - 0.07,
        "物业端车位预约信息核对",
        fontsize=18.5, fontweight="bold",
        color="#1f2850",
        fontproperties=FONT,
        zorder=5
    )

    text2 = (
        "鐗╀笟鍙€氳繃瀹㈡埛绔牳瀹炲鏉ュ仠杞︿汉鍛樹俊鎭紝闃叉鏈绾︿汉鍛樿繘鍏ュ眳浣忓尯鍚庨殢鎰廫n"
        "鍋滄斁銆佹伓鎰忓崰鐢ㄨ溅浣嶃€傚鏉ュ仠杞︿汉鍛橀渶鎻愪緵濮撳悕鍜屾墜鏈哄熬鍙凤紝鎻愬墠棰勭害锛岀墿涓歕n"
        "在客户端核实并与订单信息比对，相符则允许进入，否则禁止。"
    )
    ax.text(
        box2_x + 0.015, box2_y + box2_h - 0.11,
        text2,
        fontsize=14.5,
        color="#111111",
        linespacing=1.7,
        va="top",
        fontproperties=FONT,
        zorder=5
    )

    output = "parking_system_panel.png"
    plt.tight_layout()
    plt.savefig(output, dpi=220, bbox_inches="tight", facecolor=fig.get_facecolor())
    plt.show()
    print(f"宸茬敓鎴愬浘鐗囷細{output}")


if __name__ == "__main__":
    draw_panel()
