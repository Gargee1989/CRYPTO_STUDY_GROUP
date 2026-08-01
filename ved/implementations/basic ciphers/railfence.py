# TODO: proper testing

def enc(key, text):
    r = [[] for _ in range(key)]
    r_idx = 0
    down = True # direction
    for l in text:
        r[r_idx].append(l)
        if (r_idx + 1) >= key or (not down and r_idx - 1 < 0):
            down = not down
        r_idx += (down * 1 + (not down) * (-1))
    return r

# def dec(key, r):
#     decrypted = ""
#     r_idx = 0
#     down = True
#     while any(r):
#         # DEBUG: print(f"down: {down}\tr_idx: {r_idx}\tr: {r}")
#         decrypted += r[r_idx].pop(0) if r[r_idx] else ""
#         if (r_idx + 1) >= key or (not down and r_idx - 1) < 0:
#             down = not down
#         r_idx += (down * 1 + (-1) * (not down))
#     return decrypted

# buggy
def dec(k,s):
    if k <= 1:
        return s
    rails = [[] for _ in range(k)]
    down = True
    s_idx = 0

    for r_idx, r in enumerate(rails):
        if s_idx >= len(s):
            break
        if r_idx in (0, k - 1):
            for i in range(r_idx, len(s), 2 * (k - 1)):
                r.append(s[s_idx])
                s_idx += 1
        else:
            i = r_idx
            add_factor: int
            down = True
            while i < len(s):
                if down:
                    add_factor = 2 * (k - 1 - r_idx)
                else:
                    add_factor = 2 * r_idx
                i += add_factor
                down = not down
                r.append(s[s_idx])
                s_idx += 1

    return rails