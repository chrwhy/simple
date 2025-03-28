#include "simple_tokenizer.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <set>
#include <string>
#include <vector>

namespace simple_tokenizer {
SimpleTokenizer::SimpleTokenizer(const char **azArg, int nArg) {
  if (nArg >= 1) {
    enable_pinyin = atoi(azArg[0]) != 0;
  }
}

PinYin *SimpleTokenizer::get_pinyin() {
  static auto *py = new PinYin();
  return py;
}

static TokenCategory from_char(char c) {
  auto uc = static_cast<unsigned char>(c);
  // ASCII should in 0..127
  if (uc > 127) {
    return TokenCategory::OTHER;
  }
  if (std::isdigit(uc)) {
    return TokenCategory::DIGIT;
  }
  if (std::isspace(uc) || std::iscntrl(uc)) {
    return TokenCategory::SPACE;
  }
  if (std::isalpha(uc)) {
    return TokenCategory::ASCII_ALPHABETIC;
  }
  return TokenCategory::OTHER;
}

std::string SimpleTokenizer::tokenize_query(const char *text, int textLen, int flags) {
  int start = 0;
  int index = 0;
  std::string tmp;
  std::string result;
  while (index < textLen) {
    TokenCategory category = from_char(text[index]);
    switch (category) {
      case TokenCategory::OTHER:
        index += PinYin::get_str_len(text[index]);
        break;
      default:
        while (++index < textLen && from_char(text[index]) == category) {
        }
        break;
    }
    tmp.clear();
    std::copy(text + start, text + index, std::back_inserter(tmp));
    append_result(result, tmp, category, start, flags);
    start = index;
  }
  return result;
}

#ifdef USE_JIEBA
std::string jieba_dict_path = "./dict/";
std::string SimpleTokenizer::tokenize_jieba_query(const char *text, int textLen, int flags) {
  (void)textLen;
  static cppjieba::Jieba jieba(jieba_dict_path + "jieba.dict.utf8", jieba_dict_path + "hmm_model.utf8",
                               jieba_dict_path + "user.dict.utf8", jieba_dict_path + "idf.utf8",
                               jieba_dict_path + "stop_words.utf8");
  std::string tmp;
  std::string result;
  std::vector<cppjieba::Word> words;
  jieba.Cut(text, words);
  for (auto word : words) {
    TokenCategory category = from_char(text[word.offset]);
    append_result(result, word.word, category, word.offset, flags);
  }
  return result;
}
#endif

void SimpleTokenizer::append_result(std::string &result, std::string part, TokenCategory category, int offset,
                                    int flags) {
  if (category != TokenCategory::SPACE) {
    std::string tmp = std::move(part);
    if (category == TokenCategory::ASCII_ALPHABETIC) {
      std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c) { return std::tolower(c); });
    }

    if (flags != 0 && category == TokenCategory::ASCII_ALPHABETIC && tmp.size() > 1) {
      if (offset == 0) {
        result.append("( ");
      } else {
        result.append(" AND ( ");
      }
      std::set<std::string> pys = SimpleTokenizer::get_pinyin()->split_pinyin(tmp);
      bool addOr = false;
      for (const std::string &s : pys) {
        if (addOr) {
          result.append(" OR ");
        }
        result.append(s);
        result.append("*");
        addOr = true;
      }
      result.append(" )");
    } else {
      if (offset > 0) {
        result.append(" AND ");
      }
      if (tmp == "\"") {
        tmp += tmp;
      }
      if (category != TokenCategory::ASCII_ALPHABETIC) {
        result.append('"' + tmp + '"');
      } else {
        result.append(tmp);
      }
      if (category != TokenCategory::OTHER) {
        result.append("*");
      }
    }
  }
}

// https://cloud.tencent.com/developer/article/1198371
int SimpleTokenizer::tokenize(void *pCtx, int flags, const char *text, int textLen, xTokenFn xToken) const {
  int rc = SQLITE_OK;
  int start = 0;
  int index = 0;
  std::string result;
  while (index < textLen) {
    TokenCategory category = from_char(text[index]);
    switch (category) {
      case TokenCategory::OTHER:
        index += PinYin::get_str_len(text[index]);
        break;
      default:
        while (++index < textLen && from_char(text[index]) == category) {
        }
        break;
    }
    if (category != TokenCategory::SPACE) {
      result.clear();
      std::copy(text + start, text + index, std::back_inserter(result));
      if (category == TokenCategory::ASCII_ALPHABETIC) {
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
      }

      rc = xToken(pCtx, 0, result.c_str(), (int)result.length(), start, index);
      if (enable_pinyin && category == TokenCategory::OTHER && (flags & FTS5_TOKENIZE_DOCUMENT)) {
        const std::vector<std::string> &pys = SimpleTokenizer::get_pinyin()->get_pinyin(result);
        for (const std::string &s : pys) {
          //if (s =="li") {
          if (s=="nu" || s=="ru" || s=="tan" || s=="zhu" || s=="bi"
             || s=="che" || s=="nin" || s=="nian" || s=="nü" || s=="chu"
             || s=="na" || s=="lan" || s=="me" || s=="yin" || s=="man" || s=="ze"
             || s=="ma" || s=="wa" || s=="kua" || s=="ga" || s=="zhua" || s=="qi"
             || s=="ji" || s=="yo" || s=="qin" || s=="ben" || s=="le" || s=="sen"
             || s=="xin" || s=="jian" || s=="sa" || s=="jin" || s=="men" || s=="pan"
             || s=="gan" || s=="ti" || s=="huan" || s=="po" || s=="di" || s=="qia"
             || s=="lian" || s=="se" || s=="bin" || s=="e" || s=="lia" || s=="gen"
             || s=="zen" || s=="ran" || s=="hen" || s=="ken" || s=="kuan" || s=="pin"
             || s=="za" || s=="he" || s=="fan" || s=="te" || s=="min" || s=="xu"
             || s=="cen" || s=="zu" || s=="an" || s=="mi" || s=="she" || s=="ca"
             || s=="ju" || s=="ne" || s=="yu" || s=="lü" || s=="pi" || s=="gu"
             || s=="fo" || s=="ha" || s=="lin" || s=="yan" || s=="ba" || s=="fen"
             || s=="ce" || s=="sha" || s=="hu" || s=="a" || s=="ta" || s=="zha"
             || s=="chan" || s=="cha" || s=="xian" || s=="xia" || s=="cu" || s=="shen"
             || s=="ya" || s=="zhen" || s=="gua" || s=="wan" || s=="o" || s=="shan"
             || s=="li" || s=="shuan" || s=="ke" || s=="qu" || s=="ban" || s=="la"
             || s=="nen" || s=="guan" || s=="mo" || s=="wen" || s=="lv" || s=="can"
             || s=="su" || s=="ka" || s=="shua" || s=="shu" || s=="du" || s=="ren"
             || s=="zan" || s=="zhan" || s=="fa" || s=="nan" || s=="chuan" || s=="lu"
             || s=="pen" || s=="tu" || s=="dan" || s=="ku" || s=="re" || s=="da"
             || s=="dia" || s=="han" || s=="ge" || s=="de" || s=="en" || s=="chua"
             || s=="hua" || s=="ni" || s=="zhe" || s=="lo" || s=="xi" || s=="den"
             || s=="kan" || s=="qian" || s=="zhuan" || s=="jia" || s=="chen" || s=="nv"
             || s=="pa" || s=="san" || s=="yi") {            

            std::string appendixed = s + "v";            
            if ((int)s.length()!=1) {
                rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, appendixed.c_str(), (int)appendixed.length(), start, index);
                rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, s.c_str(), (int)s.length(), start, index);
            } else {
                rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, s.c_str(), (int)s.length(), start, index);
            }   
          } else {
            rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, s.c_str(), (int)s.length(), start, index);
          }

          if ((int)s.length()>2){
            for(int i=2;i<(int)s.length();i++){
                rc = xToken(pCtx, FTS5_TOKEN_COLOCATED, s.substr(0, i).c_str(), i, start, index);
            }
          }
        }
      }
    }
    start = index;
  }
  return rc;
}
}  // namespace simple_tokenizer
