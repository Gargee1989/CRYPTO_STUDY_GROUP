# TODO: proper testing

GREEN =     "\033[0;32m"
RED =       "\033[0;31m"
RESET =     "\033[0m"

tests = [
    (2,"HELLO WORLD","HLOWRDEL OL"),
    (2,"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG","TEQIKBONFXJMSOE H AYDGH UC RW O UP VRTELZ O"),
    (2,"Attack at dawn","Atc tdwtaka an"),
    (2,"Meet me at the station at noon","Me ea h tto tnoetm ttesaina on"),
    (2,"Python programming is fun","Pto rgamn sfnyhnpormigi u"),
    (3,"WE ARE DISCOVERED FLEE AT ONCE","WRIVDETCEAEDSOEE LEA NE  CRF O"),
    (3,"DEFEND THE EAST WALL","DNHAWEEDTEES ALF  TL"),
    (3,"Hello World!","Horel ol!lWd"),
    (3,"The rain in Spain falls mainly on the plain.","Tr Snlmlneah ani pi al anyo h li.einafsi tpn"),
    (3,"Encryption and decryption should be reversible.","Eyonepno rrlnrpinaddcyto hudb eesbect  rislevi."),
    (3,"This is only a test.","T o ehsi nyats.isl t"),
    (4,"Knowledge is power.","Kd .negsproleioew w"),
    (4,"Sphinx of black quartz judge my vow.","S aauypxolcurjdm .hnfbkqt g vwi  zeo"),
    (4,"Pack my box with five dozen liquor jugs.","Pywizquam  ifvoeiujgc bxt ednlo skoh  r."),
    (4,"Never odd or even","No e drevrdovne e"),
    (4,"0123456789","0615724839"),
    (4,"A longer message with multiple words and punctuation!","Aesil  ut grsawtutewsapnailn eg hmilodn cuo!ome prdtn"),
    (5,"The five boxing wizards jump quickly.","T wjcheb i uikevogzsmul ixnadpqyfir ."),
    (5,"Far above the world so high","Feriav olhgrotwd h bh  oaes"),
    (5,"To be or not to be, that is the question.","T b q.orn eti un ooo,aseeob tt h hsie ttt"),
    (5,"ABCDEFGHIJKLMNOPQRSTUVWXYZ","AIQYBHJPRXZCGKOSWDFLNTVEMU"),
    (5,"abcdefghijklmnopqrstuvwxyz","aiqybhjprxzcgkoswdflntvemu"),
    (6,"Rail fence ciphers are classical transposition ciphers.","R rashaecaecloipeici  i ptirlnpscstsics ehrlsrno .feaan"),
    (6,"Testing edge cases improves software quality.","Tgmfledeipotaise  rswutt cso aqyigaevsr .nsee"),
    (6,"One fish two fish red fish blue fish","Owdentoe u e  rflf hf ibifsihs sishh"),
    (6,"Lorem ipsum dolor sit amet consectetur adipiscing elit.","Lmtsdeou i neai lrsdsaoc pgiepo mctrintmilre eusi. ottc"),
    (7,"Data structures and algorithms","Drraueoittsgtac lh uaamsrn std"),
    (7,"The answer is 42.","Tshi e 4 r2ae.nws"),
    (7,"A journey of a thousand miles begins with a single step.","A m l fa iswgejo dlnin o tneitisuyhasghstreos e  e.nubap"),
    (7,"OpenAI develops powerful AI models.","Oo plplAeesuI.nv f sAeprmlIdoeoe wd"),
    (8,"Symmetric encryption uses the same key.","Syeyrph mcttsmni aeeosm.t neeyrc s eiuk"),
    (8,"Unit tests should be deterministic.","Uunnoliiihdmsts rt  beitsetcet e.sd"),
    (8,"Every good boy deserves fudge.","E evydg.eoedrbsuy ef dr govsoe"),
    (8,"How vexingly quick daft zebras jump!","Huaoqirsw cb  ykejvl zuegd mxnatpif!"),
    (9,"Complex inputs help uncover implementation bugs.","Ceeohllm.m ppesps mngltuitueun abxpcrt  noeinivo"),
    (9,"Short","Short"),
    (9,"Medium length example sentence.","Maexmdep.i leuhecmt n gselneten"),
    (10,"This plaintext is intentionally somewhat longer than the others.","Tiwhh nehteistma  sieotno  ns atptt lhhlxiyoteaeoln ritnlgrsnae."),
    (10,"123 Main Street, Springfield, ZIP 12345", "1p32Sr243 i15 ,n MtgPaefIieiZnre  tl,Sd"),
    (10,"Do not forget to validate your inputs.","Dasovlt.  iunodpotant ti te fe rogyuro"),
    (11,"A","A"),
    (11,"AB","AB"),
    (11,"ABC","ABC"),
    (11,"ABCD","ABCD"),
    (12,"The cake is a lie.","The ca.keei li sa "),
    (12,"Spaces     with     irregular     gaps","Srpreaigc ue ls a  r   s h p t a i gw "),
    (13,"Mixing UPPER and lower CASE letters.","MAiCSx Eir nelgwe otUltP ePdrEnsRa. "),
    (13,"Edge-case verification is important.","Esdi g ienm-opcioatrsateca invftei.r"),
    (14,"An unusually high rail count for a short message.","Annut o ucfn o.ulresi guaaaar sl sslhheygom ir ht"),
    (15,"Rail Fence Cipher","Rail Fence Cirpeh"),
    (16,"Testing with sixteen rails.","Testi.nsgl iwairt hn eseitx"),
    (20,"Very high key compared to message length.","Vteghrn.ye lh ieggha skseeym  cootm pdaer"),
    (25,"Tiny","Tiny"),
    (30,"Another tiny message","Another tiny message")
]


def enc(key, text):
    r = [[] for i in range(key)]
    r_idx = 0
    down = True # direction
    for l in text:
        r[r_idx].append(l)
        if (r_idx + 1) >= key or (not down and r_idx - 1 < 0):
            down = not down
        r_idx += (down * 1 + (not down) * (-1))
    return r

def dec(key, r):
    decrypted = ""
    r_idx = 0
    down = True
    while any(r):
        # DEBUG: print(f"down: {down}\tr_idx: {r_idx}\tr: {r}")
        decrypted += r[r_idx].pop(0) if r[r_idx] else ""
        if (r_idx + 1) >= key or (not down and r_idx - 1) < 0:
            down = not down
        r_idx += (down * 1 + (-1) * (not down))
    return decrypted

for key, plain, cipher in tests:
    if cipher == "".join("".join(x) for x in enc(key, plain)):
        print(f"{GREEN}[PASS]{RESET}")
    else:
        print(f"{RED}[FAIL]{RESET}: ({key}, {plain}, {cipher})")