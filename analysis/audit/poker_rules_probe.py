"""Finite rank/suit-class comparison, not an execution of the Win16 binary.

Original translations: 1010:1f9a groups, 20c2 straight, 21f0 flush,
224a royal; native model follows game_data.cpp's current payout block.
All rank multisets possible in one deck, wild ranks 0/2/7, and both
realizable non-wild suit classes are covered. Card order is irrelevant to
these classifiers. Dealing, betting, and RNG are deliberately out of scope.
"""
from collections import Counter
from itertools import combinations_with_replacement


def original_groups(counts, wilds, wild_rank):
    pair = triple = high_pair = False
    for rank in range(1, 14):
        if rank == wild_rank:
            continue
        count = counts[rank]
        if count + wilds == 5:
            return "five"
        if count + wilds == 4:
            return "four"
        if count == 3:
            if pair:
                return "full"
            triple = True
        elif count == 2:
            if triple or (wilds > 0 and pair):
                return "full"
            if wilds == 0 and pair:
                return "two"
            high_pair |= rank in (1, 11, 12, 13)
            pair = True
    if triple or (pair and wilds == 1) or wilds == 2:
        return "three"
    return "pair" if high_pair else "none"


def original_straight(counts, wilds):
    present = [bool(counts[rank]) for rank in range(1, 14)]
    present.append(present[0])
    first = next((i for i in range(13) if present[i]), -1)
    def run(start, last):
        missing = sum(i > last or not present[i] for i in range(start + 1, start + 5))
        return missing <= wilds
    result = run(first, 12)
    if first == 0 and not result:
        first = next((i for i in range(1, 14) if present[i]), -1)
        result = run(first, 13)
    return result


def payout(hand, wild_rank, flush, original):
    counts = Counter(rank for rank in hand if rank != wild_rank)
    wilds = hand.count(wild_rank)
    if original:
        straight = original_straight(counts, wilds)
        royal = straight and flush and min((r for r in counts if r > 1), default=255) >= 10
        group = original_groups(counts, wilds, wild_rank)
    else:
        straight = any(sum(not counts[r] for r in range(high - 4, high + 1)) <= wilds
                       for high in range(5, 14))
        royal = flush and sum(not counts[r] for r in (1, 10, 11, 12, 13)) <= wilds
        most = max(counts.values(), default=0) + wilds
        full = any(max(0, 3 - counts[a]) + max(0, 2 - counts[b]) <= wilds
                   for a in range(1, 14) for b in range(1, 14) if a != b)
        two = any(max(0, 2 - counts[a]) + max(0, 2 - counts[b]) <= wilds
                  for a in range(1, 14) for b in range(1, 14) if a != b)
        group = ("five" if most >= 5 else "four" if most >= 4 else "full" if full
                 else "three" if most >= 3 else "two" if two else "pair" if most >= 2 else "none")
    category = ("royal" if royal else "five" if group == "five" else "straight_flush" if straight and flush
                else "four" if group == "four" else "full" if group == "full" else "flush" if flush
                else "straight" if straight else group)
    table = dict(zip(("royal", "five", "straight_flush", "four", "full", "flush", "straight", "three", "two", "pair", "none"),
                     (200, 25, 100, 40, 12, 10, 8, 5, 2, 1, 0) if wild_rank == 0
                     else (25, 25, 20, 12, 6, 5, 4, 2, 1, 0, 0)))
    return table[category]


if __name__ == "__main__":
    cases = 0
    differences = Counter()
    examples = {}
    for hand in combinations_with_replacement(range(1, 14), 5):
        if max(Counter(hand).values()) > 4:
            continue
        for wild_rank in (0, 2, 7):
            nonwild = Counter(r for r in hand if r != wild_rank)
            for flush in (False, True):
                if flush and max(nonwild.values(), default=0) > 1:
                    continue  # Duplicate same-rank non-wild cards cannot share a suit.
                if not flush and sum(nonwild.values()) < 2:
                    continue
                cases += 1
                old = payout(hand, wild_rank, flush, True)
                new = payout(hand, wild_rank, flush, False)
                if old != new:
                    key = (wild_rank, flush, old, new)
                    differences[key] += 1
                    examples.setdefault(key, hand)
    print(f"legal rank/suit-class cases={cases} mismatches={sum(differences.values())}")
    for key, count in sorted(differences.items()):
        print(f"wild,flush,original,native={key} count={count} example={examples[key]}")
