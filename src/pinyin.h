#ifndef PINYIN_H_
#define PINYIN_H_

#include <map>
#include <set>
#include <string>
#include <vector>

namespace simple_tokenizer {

class PinYin {
 private:
  std::map<int, std::vector<std::string> > pinyin;
  std::map<std::string, std::string> ts;
  const std::vector<std::string> empty_vector;
  std::set<std::string> all_pinyins;
  std::map<std::string, std::string> sub_pinyins;
  /*
  {
        {"nv","nv"}, {"pan","pan"}, {"san","san"}, {"cen","cen"}, {"ku","ku"}, 
        {"ru","ru"}, {"wa","wa"}, {"ben","ben"}, {"ma","ma"}, {"lan","lan"}, 
        {"xu","xu"}, {"ca","ca"}, {"ba","ba"}, {"lian","lian"}, {"zan","zan"}, 
        {"po","po"}, {"wen","wen"}, {"lo","lo"}, {"bin","bin"}, {"can","can"}, 
        {"kuan","kuan"}, {"yu","yu"}, {"xi","xi"}, {"an","an"}, {"lin","lin"}, 
        {"dan","dan"}, {"zhua","zhua"}, {"gua","gua"}, {"hu","hu"}, {"ke","ke"}, 
        {"zhe","zhe"}, {"pin","pin"}, {"tan","tan"}, {"na","na"}, {"qin","qin"}, 
        {"shan","shan"}, {"xian","xian"}, {"de","de"}, {"chan","chan"}, {"ga","ga"}, 
        {"chua","chua"}, {"du","du"}, {"zhen","zhen"}, {"ne","ne"}, {"lu","lu"}, 
        {"ha","ha"}, {"yo","yo"}, {"bi","bi"}, {"fan","fan"}, {"mo","mo"}, 
        {"ta","ta"}, {"men","men"}, {"da","da"}, {"ge","ge"}, {"lia","lia"}, 
        {"jin","jin"}, {"xin","xin"}, {"chu","chu"}, {"ya","ya"}, {"fo","fo"}, 
        {"ken","ken"}, {"ban","ban"}, {"me","me"}, {"se","se"}, {"ren","ren"}, 
        {"ni","ni"}, {"pen","pen"}, {"zu","zu"}, {"den","den"}, {"guan","guan"}, 
        {"pa","pa"}, {"la","la"}, {"zhu","zhu"}, {"gan","gan"}, {"sha","sha"}, 
        {"lv","lv"}, {"en","en"}, {"hen","hen"}, {"he","he"}, {"nin","nin"}, 
        {"ran","ran"}, {"te","te"}, {"yin","yin"}, {"shuan","shuan"}, {"xia","xia"}, 
        {"qia","qia"}, {"ti","ti"}, {"qian","qian"}, {"han","han"}, {"re","re"}, 
        {"fa","fa"}, {"nu","nu"}, {"e","e"}, {"mi","mi"}, {"ju","ju"}, 
        {"man","man"}, {"pi","pi"}, {"shen","shen"}, {"di","di"}, {"shua","shua"}, 
        {"kan","kan"}, {"jia","jia"}, {"dia","dia"}, {"hua","hua"}, {"ka","ka"}, 
        {"za","za"}, {"nan","nan"}, {"kua","kua"}, {"chuan","chuan"}, {"wan","wan"}, 
        {"jian","jian"}, {"lü","lü"}, {"zen","zen"}, {"cha","cha"}, {"o","o"}, 
        {"ze","ze"}, {"che","che"}, {"cu","cu"}, {"li","li"}, {"nen","nen"}, 
        {"sen","sen"}, {"she","she"}, {"ce","ce"}, {"huan","huan"}, {"qi","qi"}, 
        {"su","su"}, {"nian","nian"}, {"zhuan","zhuan"}, {"zhan","zhan"}, {"chen","chen"}, 
        {"min","min"}, {"shu","shu"}, {"tu","tu"}, {"qu","qu"}, {"zha","zha"}, 
        {"nü","nü"}, {"le","le"}, {"fen","fen"}, {"ji","ji"}, {"a","a"}, 
        {"yan","yan"}, {"sa","sa"}, {"gu","gu"}, {"gen","gen"}, {"yi","yi"}
    }
 */


  // clang-format off
  const std::map<std::string, char> tone_to_plain = {
      {"ā", 'a'}, {"á", 'a'}, {"ǎ", 'a'}, {"à", 'a'},
      {"ē", 'e'}, {"é", 'e'}, {"ě", 'e'}, {"è", 'e'},
      {"ế", 'e'}, {"ề", 'e'}, {"ê", 'e'},
      {"ō", 'o'}, {"ó", 'o'}, {"ǒ", 'o'}, {"ò", 'o'},
      {"ī", 'i'}, {"í", 'i'}, {"ǐ", 'i'}, {"ì", 'i'},
      {"ū", 'u'}, {"ú", 'u'}, {"ǔ", 'u'}, {"ù", 'u'},
      {"ǘ", 'u'}, {"ǚ", 'u'}, {"ǜ", 'u'}, {"ü", 'u'},
      {"ń", 'n'}, {"ň", 'n'}, {"ǹ", 'n'},
      {"ḿ", 'm'},
  };
  // 不是合法拼音，但是是前缀，只能出现在结尾。
  const std::set<std::string> pinyin_prefix = {
        "be","bia",
        "ch","cho","chon","chua","co","con","cua",
        "din","don","do","dua",
        "fe",
        "go","gon",
        "ho","hon",
        "len","lon","lua",
        "mia",
        "nia","no","non","nua",
        "pe","pia",
        "qio","qion","qua",
        "ra","ro","ron","rua",
        "sh","sho","so","son","sua",
        "ten","tia","tin","to","ton","tua",
        "we",
        "xio","xion","xua",
        "yon","yua",
        "zh","zho","zhon","zo","zon","zua",
    };

    // 合法拼音
  const std::set<std::string> pinyin_valid = {
        "a", "ai", "an", "ang", "ao",
        "ba", "bai", "ban", "bang", "bao",
              "bei", "ben", "beng", "bi", "bian", "biao", "bie", "bin", "bing", "bo", "bu",
        "ca", "cai", "can", "cang", "cao", "ce", "cen", "ceng", "cha", "chai",
              "chan", "chang", "chao", "che", "chen", "cheng", "chi", "chong", "chou", "chu",
              "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci", "cong", "cou", "cu",
              "cuan", "cui", "cun", "cuo",
        "da", "dai", "dan", "dang", "dao", "de", "dei",
              "den", "deng", "di", "dia", "dian", "diao", "die", "ding", "diu",
              "dong", "dou", "du", "duan", "dui", "dun", "duo",
        "e", "ei", "en", "eng", "er",
        "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu",
        "ga", "gai", "gan", "gang", "gao", "ge", "gei", "gen", "geng",
              "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun", "guo",
        "ha", "hai", "han", "hang", "hao", "he",
              "hei", "hen", "heng", "hong", "hou", "hu",
              "hua", "huai", "huan", "huang", "hui", "hun", "huo",
        // "i"=>[],
        "ji", "jia", "jian", "jiang", "qiao", "jiao", "jie", "jin", "jing", "jiong",
              "jiu", "ju", "juan", "jue", "jun","jv",
        "ka", "kai", "kan", "kang", "kao", "ke", "kei", "ken", "keng", "kong", "kou", "ku", "kua", "kuai",
              "kuan", "kuang", "kui", "kun", "kuo",
        "la", "lai", "lan", "lang", "lao",
              "le", "lei", "leng", "li", "lia", "lian", "liang", "liao", "lie", "lin",
              "ling", "liu", "long", "lo", "lou", "lu", "luan", "lue", "lun", "luo","lv",
        "ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian",
              "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu",
        "na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian", "niang",
              "niao", "nie", "nin", "ning", "niu", "nong", "nou", "nu", "nuan", "nue", "nun", "nuo", "nv",
        "o", "ou",
        "pa", "pai", "pan", "pang", "pao", "pei", "pen",
              "peng", "pi", "pian", "piao", "pie", "pin", "ping", "po", "pou", "pu",
        "qi", "qia", "qian", "qiang", "qie", "qin", "qing", "qiong", "qiu", "qu",
              "quan", "que", "qun","qv",
        "ran", "rang", "rao", "re", "ren", "reng", "ri",
              "rong", "rou", "ru", "ruan", "rui", "run", "ruo",
        "sa", "sai", "san",
              "sang", "sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao",
              "she", "shei", "shen", "sheng", "shi", "shou", "shu", "shua", "shuai", "shuan", "shuang",
              "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui", "sun", "suo",
        "ta", "tai", "tan", "tang", "tao", "te", "tei", "teng", "ti", "tian",
              "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo",
        // "u"=>[],
        // "v"=>[],
        "wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu",
        "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing", "xiong", "xiu", "xu",
              "xuan", "xue", "xun","xv",
        "ya", "yan", "yang","yao", "ye", "yi", "yin", "ying",
              "yo", "yong", "you", "yu", "yuan", "yue", "yun",
        "za", "zai", "zan",
              "zang", "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang",
              "zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai",
              "zhuan", "zhuang", "zhui", "zhun", "zhuo", "zi", "zong", "zou", "zu", "zuan",
        "zui", "zun", "zuo",
    };
  // clang-format on
  std::set<std::string> to_plain(const std::string &input);
  std::map<int, std::vector<std::string> > build_pinyin_map();
  std::map<std::string, std::string> build_ts_map();
  static int codepoint(const std::string &u);
  std::vector<std::string> _split_pinyin(const std::string &input, int begin, int end);

 public:
  const std::vector<std::string> &get_pinyin(const std::string &chinese);
  const std::string &get_ts(const std::string &t);
  const bool &is_sub_pinyin(const std::string &pinyin);
  static int get_str_len(unsigned char byte);
  std::set<std::string> split_pinyin(const std::string &input);
  PinYin();
};

}  // namespace simple_tokenizer

#endif  // PINYIN_H_
