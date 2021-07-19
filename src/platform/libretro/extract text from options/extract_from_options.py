#!/usr/bin/env python3

"""Core options text extractor

The purpose of this script is to aid in the conversion of
'struct retro_core_option_definition option_defs_us' within
libretro_core_options.h into a form compatible with the existing
scripts for Crowdin synchronization.

This script accepts a text file named [name_of_core]_us.txt
containing a copy of the entire option_defs_us struct.

This script will:
1.) create key words for & extract most of the relevant texts from option_defs_us:
    the description, the info texts & (almost) all options
2.) create a msg_hash.h file declaring all keys within an enum
    as well as implementing all functions needed
3.) create a msg_hash_[input_file_name].h containing the key/string pairs,
    as well as msg_hash dummy files for all supported languages in a
    ./intl subdirectory
4.) create a [input_file_name]_code.txt containing the converted
    option_defs struct

After the conversion finishes, the ./intl directory and the msg_hash.h
file need to be placed into the same directory libretro_core_options.h
is in. The original option_defs_us & option_defs_intl structs should be deleted
and the new option_defs struct copied into the libretro_set_core_options()
function. The code then needs to be adjusted for the changes
(include msg_hash.h & remove no longer needed code, etc.).
option_defs should replace all instances of option_defs_us and
option_defs_intl.

NOTE: This script has a few limitations. It is not really compatible with
macros. It does not fix bad source text. The key words chosen might be inadequate.
All of these issues require manual touch-ups.
This script also skips all instances of 'on', 'off', 'true', 'false',
'enable' and 'disable', because those are translated by RetroArch.
"""

import re
import sys
import os

try:
    # get input file name
    h_filename = sys.argv[1]
    # set output file names
    filename = os.path.splitext(h_filename)[0]
    hash_filename = 'msg_hash.h'
    msg_filename = 'msg_hash_' + filename + '.h'
    code_filename = filename + '_code.txt'

    intl_files = {
        'ARABIC': 'msg_hash_' + filename[:-2] + 'ar.h',
        'ASTURIAN': 'msg_hash_' + filename[:-2] + 'ast.h',
        'CHINESE_SIMPLIFIED': 'msg_hash_' + filename[:-2] + 'chs.h',
        'CHINESE_TRADITIONAL': 'msg_hash_' + filename[:-2] + 'cht.h',
        'DUTCH': 'msg_hash_' + filename[:-2] + 'nl.h',
        'ENGLISH': 'msg_hash_' + filename[:] + '.h',
        'ESPERANTO': 'msg_hash_' + filename[:-2] + 'eo.h',
        'FINNISH': 'msg_hash_' + filename[:-2] + 'fi.h',
        'FRENCH': 'msg_hash_' + filename[:-2] + 'fr.h',
        'GERMAN': 'msg_hash_' + filename[:-2] + 'de.h',
        'GREEK': 'msg_hash_' + filename[:-2] + 'el.h',
        'HEBREW': 'msg_hash_' + filename[:-2] + 'he.h',
        'ITALIAN': 'msg_hash_' + filename[:-2] + 'it.h',
        'JAPANESE': 'msg_hash_' + filename[:-2] + 'jp.h',
        'KOREAN': 'msg_hash_' + filename[:-2] + 'ko.h',
        'PERSIAN': 'msg_hash_' + filename[:-2] + 'fa.h',
        'POLISH': 'msg_hash_' + filename[:-2] + 'pl.h',
        'PORTUGUESE_BRAZIL': 'msg_hash_' + filename[:-2] + 'pt_br.h',
        'PORTUGUESE_PORTUGAL': 'msg_hash_' + filename[:-2] + 'pt_pt.h',
        'RUSSIAN': 'msg_hash_' + filename[:-2] + 'ru.h',
        'SLOVAK': 'msg_hash_' + filename[:-2] + 'sk.h',
        'SPANISH': 'msg_hash_' + filename[:-2] + 'es.h',
        'TURKISH': 'msg_hash_' + filename[:-2] + 'tr.h',
        'VIETNAMESE': 'msg_hash_' + filename[:-2] + 'vn.h'
    }

except IndexError:
    print("Usage: python3 ./extract_from_options.py path/to/[name_of_core]_us.txt\n")
    sys.exit(1)

p = re.compile(
    r'\{\s*(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+\s*)*'  # opening braces 
    r'(?:\".*?\"|[a-zA-Z0-9_]+\s*[(\[](?:.|[\r\n])*?[)\]]|[a-zA-Z0-9_]+\s*\".*?\")\s*'  # key
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,\s*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'
    r'\".*?\"\s*'  # description
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,\s*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'
    r'(?:\"(?:.|[\r\n])*?\"|NULL)\s*'  # info
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,\s*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'
    r'\{\s*(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'  # opening braces
    r'(?:\{\s*(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'  # opening braces
    r'(?:\".*?\"|NULL)\s*'  # option key
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,\s*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'
    r'(?:\".*?\"|NULL)\s*'  # option value
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*\}\s*'  # closing braces
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,?\s*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*)*\}\s*'  # closing braces
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,?\s*'  # comma
    r'(?:(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*'
    r'(?:\".*?\"|NULL)\s*'  # default value
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*,?\s*)*'  # comma
    r'(?:(?:\/\*(?:.|[\r\n])*?\*\/|\/\/.*[\r\n]+|#.*[\r\n]+)\s*)*\},'  # closing braces
)

sp_chars = []
for unicode in range(33, 48):
    sp_chars.append(chr(unicode))
for unicode in range(58, 65):
    sp_chars.append(chr(unicode))
for unicode in range(91, 97):
    if unicode == 95:
        continue
    sp_chars.append(chr(unicode))
for unicode in range(123, 127):
    sp_chars.append(chr(unicode))

on_offs = ['"enabled"', '"disabled"', '"true"', '"false"', '"on"', '"off"']


def min_open_close(message: str,
                   open1_idx: int, close1_token: str,
                   open2_idx: int, close2_token: str) -> [int, int]:
    """Determines the earlier occurring opening token and it's closing token position.

    :param message: The text in which to search for the closing tokens.
    :param open1_idx: The position of the first opening token in message.
    :param close1_token: The closing token to the first opening token, e.g. '*/'.
    :param open2_idx: The position of the second opening token in message.
    :param close2_token: The closing token to the second opening token, e.g. '\n'.
    :return: The positions of the open/close token combo of the earlier occurring opening token.
             If both open indices are negative returns [-1, -1] tuple.
    """

    if 0 > open1_idx and 0 > open2_idx:
        open_idx = -1
        close_idx = -1
    elif 0 > open2_idx:
        open_idx = open1_idx
        close_idx = message.find(close1_token, open1_idx + 1)
    elif 0 > open1_idx:
        open_idx = open2_idx
        close_idx = message.find(close2_token, open2_idx + 1)
    else:
        if open1_idx < open2_idx:
            open_idx = open1_idx
            close_idx = message.find(close1_token, open1_idx + 1)
        else:
            open_idx = open2_idx
            close_idx = message.find(close2_token, open2_idx + 1)

    return open_idx, close_idx


def max_open_close(message: str,
                   open1_idx: int, close1_token: str,
                   open2_idx: int, close2_token: str,
                   srch_to: int = -1) -> [int, int]:
    """Determines the later occurring, or enclosing, opening token and it's closing token position.

    :param message: The text in which to search for the closing tokens.
    :param open1_idx: The position of the first opening token in message.
    :param close1_token: The closing token to the first opening token, e.g. '*/'.
    :param open2_idx: The position of the second opening token in message.
    :param close2_token: The closing token to the second opening token, e.g. '\n'.
    :param srch_to: The maximum position in message up to which to search.
    :return: The positions of the open/close token combo that either occurs later or encloses the other open token.
             If both open indices are negative returns [-1, -1] tuple.
    """

    if 0 > open1_idx and 0 > open2_idx:
        open_idx = -1
        close_idx = -1
    elif 0 > open2_idx:
        open_idx = open1_idx
        close_idx = message.find(close1_token, open1_idx, srch_to)
    elif 0 > open1_idx:
        open_idx = open2_idx
        close_idx = message.find(close2_token, open2_idx, srch_to)
    else:
        b1 = message.find(close1_token, open1_idx, srch_to)
        b2 = message.find(close2_token, open2_idx, srch_to)
        if open1_idx < open2_idx < b1 or b2 < open1_idx:
            open_idx = open1_idx
            close_idx = b1
        elif open2_idx < open1_idx < b2 or b1 < open2_idx:
            open_idx = open2_idx
            close_idx = b2
        else:
            print('A weird error has occurred in determine_reversed_a_b()!\n')
            sys.exit(0)

    return open_idx, close_idx


def min_idx(idx1: int, idx2: int) -> int:
    """Determines the smaller index.

    :param idx1: First numerical index.
    :param idx2: Second numerical index.
    :return: The smaller non-negative index or, should both be negative, -1.
    """

    if 0 <= idx1 and 0 <= idx2:
        if idx1 < idx2:
            out_idx = idx1
        else:
            out_idx = idx2
    elif 0 <= idx1:
        out_idx = idx1
    elif 0 <= idx2:
        out_idx = idx2
    else:
        out_idx = -1
    return out_idx


def find_second_quote(message: str, quote1_idx: int) -> int:
    """Finds the next double quote after quote1_idx in message which is not escaped.

    :param message: The text in which to search.
    :param quote1_idx: The position of the first double quote (start of string) in message.
    :return: The position of the next matching double quote (end of string).
             Returns -1 if quote1_idx is negative or no matching quote is found.
    """

    if 0 <= quote1_idx:
        second_quote: int = message.find('"', quote1_idx + 1)
        while message[second_quote - 1] == '\\':
            second_quote = message.find('"', second_quote + 1)
        return second_quote
    else:
        return -1


def find_string(message: str, srch_from: int = 0) -> [int, int]:
    """Finds the first double quotes enclosed string.

    :param message: The text in which to search.
    :param srch_from: The position from which to search.
    :return: The positions of the opening and closing double quotes (start & end of string).
             Returns (-1, -1) if no string is found.
    """

    # skip all preceding comments first
    single_line = message.find('//', srch_from)
    multi_line = message.find('/*', srch_from)
    first_quote = message.find('"', srch_from)

    a, b = min_open_close(message, single_line, '\n', multi_line, '*/')

    while 0 <= a <= b and not (0 <= first_quote < a):
        single_line = message.find('//', b + 1)
        multi_line = message.find('/*', b + 1)
        first_quote = message.find('"', b + 1)
        a, b = min_open_close(message, single_line, '\n', multi_line, '*/')

    second_quote = find_second_quote(message, first_quote)

    return first_quote, second_quote


def rfind_string(message: str, srch_from: int = 0, srch_to: int = -1) -> int:
    """Finds the last occurrence of double quotes in message[srch_from:srch_to] outside of comments.

    :param message: The text to search in.
    :param srch_from: The position from which to search.
    :param srch_to: The position up to which to search.
    :return: The index of the last occurrence of double quotes.
             Returns -1 if no double quotes are found found.
    """

    # skip all preceding comments first
    single_line = message.rfind('//', srch_from, srch_to)
    multi_line = message.rfind('/*', srch_from, srch_to)
    last_quote = message.rfind('"', srch_from, srch_to)

    a, b = max_open_close(message, single_line, '\n', multi_line, '*/', srch_to)

    while 0 <= a <= b and not (0 <= b < last_quote):
        single_line = message.rfind('//', srch_from, a)
        multi_line = message.rfind('/*', srch_from, a)
        last_quote = message.rfind('"', srch_from, a)
        a, b = max_open_close(message, single_line, '\n', multi_line, '*/', a)

    return last_quote


def find_string_or_null(message: str, srch_from: int = 0) -> [int, int, int]:
    """Finds the first instances of a double quotes enclosed string and NULL key word.

    :param message: The text to search in.
    :param srch_from: The position from which to search.
    :return: The two indices of the first occurrences of matching double quotes (string)
             and the index of the first occurrence of NULL.
             Returns -1 for the corresponding value if it is not found.
    """

    # remove all preceding comments first
    single_line = message.find('//', srch_from)
    multi_line = message.find('/*', srch_from)
    first_quote = message.find('"', srch_from)
    nul = message.find('NULL', srch_from)

    a, b = min_open_close(message, single_line, '\n', multi_line, '*/')
    c = min_idx(first_quote, nul)

    while 0 <= a <= b and not (0 <= c < a):
        single_line = message.find('//', b + 1)
        multi_line = message.find('/*', b + 1)
        first_quote = message.find('"', b + 1)
        nul = message.find('NULL', b + 1)
        a, b = min_open_close(message, single_line, '\n', multi_line, '*/')
        c = min_idx(first_quote, nul)

    second_quote = find_second_quote(message, first_quote)

    return first_quote, second_quote, nul


def find_token(message: str, token: str, srch_from: int = 0):
    """Finds a not commented token outside of strings.

    :param message: The text to search in.
    :param token: The token to search for. Can be a single character or multiple contiguous characters.
    :param srch_from: The position from which to search.
    :return: The position of the first token found in message.
             Returns -1 if token is not found.
    """

    q1, q2 = find_string(message, srch_from)  # string is outside of comments
    token_idx = message.find(token, srch_from)
    single_line = message.find('//', srch_from)
    multi_line = message.find('/*', srch_from)
    a, b = min_open_close(message, single_line, '\n', multi_line, '*/')

    while (0 <= a <= b and not 0 <= token_idx < a) or (0 <= q1 < q2 and not 0 <= token_idx < q1):
        # find a token outside of comments
        while 0 <= a <= b and not 0 <= token_idx < a:
            single_line = message.find('//', b + 1)
            multi_line = message.find('/*', b + 1)
            token_idx = message.find(token, b + 1)
            a, b = min_open_close(message, single_line, '\n', multi_line, '*/')
        # find a token outside of string
        while 0 <= q1 <= q2 and not 0 <= token_idx < q1:
            token_idx = message.find(token, q2 + 1)
            q1, q2 = find_string(message, q2 + 1)

    return token_idx


def find_closing_bracket(message: str, start_idx: int, start_bracket: str) -> int:
    """Finds the next closing bracket in message matching start_bracket after start_idx.

    :param message: The text in which to search.
    :param start_idx: The position of the opening bracket.
    :param start_bracket: The opening bracket.
    :return: The position of the matching closing bracket.
             Returns -1 if no matching bracket is found or start_idx is negative.
    """

    bracket_match = {
        '(': ')',
        '{': '}',
        '[': ']',
    }
    if 0 <= start_idx:
        closing_bracket = find_token(message, bracket_match[start_bracket], start_idx + 1)
        check = find_token(message, start_bracket, start_idx + 1)

        while 0 <= check < closing_bracket:
            closing_bracket = find_token(message, bracket_match[start_bracket], closing_bracket + 1)
            check = find_token(message, start_bracket, closing_bracket + 1)
        return closing_bracket
    else:
        return -1


def find_func(message: str, srch_from: int = 0) -> [int, int]:
    """Find the first function (parenthesis), array (brackets) or string (double quotes).

    :param message: The text to search in.
    :param srch_from: The position from which to search.
    :return: The start and end position of the found instance.
             Returns (-1, -1) if nothing is found.
    """

    # consider all preceding comments first
    single_line = message.find('//', srch_from)
    multi_line = message.find('/*', srch_from)

    para_start = message.find('(', srch_from)  # function
    bracket_start = message.find('[', srch_from)  # array
    first_quote = message.find('"', srch_from)  # string

    a, b = min_open_close(message, single_line, '\n', multi_line, '*/')
    start = min_idx(first_quote, para_start)
    start = min_idx(start, bracket_start)

    while 0 <= a <= b and not (0 <= start < a):
        single_line = message.find('//', b + 1)
        multi_line = message.find('/*', b + 1)

        para_start = message.find('(', b + 1)
        bracket_start = message.find('[', b + 1)
        first_quote = message.find('"', b + 1)

        a, b = min_open_close(message, single_line, '\n', multi_line, '*/')
        start = min_idx(para_start, first_quote)
        start = min_idx(start, bracket_start)

    if message[start] == '"':
        end = find_second_quote(message, start)
    elif message[start] == '(':
        end = find_closing_bracket(message, start, '(')
    elif message[start] == '[':
        end = find_closing_bracket(message, start, '[')
    else:
        print('Failed at finding a match! (In find_func())\n')
        sys.exit(0)

    return start, end


def parse_message(message: str):
    """Extracts the constant text (quoted strings) from a retro_core_option_definition element.
    Skips any instances of 'on', 'off', 'true', 'false', 'enable' and 'disable'.

    :param message: The text to be parsed.
    :return: An array containing the extracted strings with the general form
             [option key, description/label, [info text(s), ...], key0, value0, key1, value1, ...].
             A dictionary of the form {start of string: end of string
             (for everything up to & including the info texts & non-null option values),
             ..., start of key0: {start of null0, end of null0} (for NULL option values), ...}.
    """

    res = []  # resulting array of strings
    msg_ids = {}
    # get the key; could be string or contents of 'func(contents)'
    str_start, str_end = find_func(message)
    res.append(message[str_start + 1:str_end])  # don't include double quotes/brackets

    # from here: include double quotes for strings
    # get the description; definitely a string
    str_start, str_end = find_string(message, str_end + 1)
    str_end = str_end + 1
    res.append(message[str_start:str_end])
    msg_ids[str_start] = str_end

    # get the information, could be one or more (multiline) strings or NULL
    infos = []
    brace = find_token(message, '{', str_end)
    str_start, str_end, nul = find_string_or_null(message, str_end)
    while brace > str_end or brace > nul:
        if 0 <= str_start < nul or nul < 0 <= str_start:
            comma = find_token(message, ',', str_end + 1)
            true_end = rfind_string(message, str_end, comma)
            str_end = true_end + 1
            infos.append(message[str_start:str_end])
            msg_ids[str_start] = str_end
        elif 0 <= nul < str_start or str_start < 0 <= nul:
            str_end = nul + 4
            infos.append(message[nul:str_end])
            msg_ids[nul] = str_end
        else:  # should never occur
            print('Found no strings or NULLs while looking for info text! How is that even possible?\n'
                  '(In parse_message())\n')
            sys.exit(0)
        str_start, str_end, nul = find_string_or_null(message, str_end)
    res.append(infos)

    # get the options; key-value pairs
    while True:
        # get key
        if 0 <= str_start < nul or nul < 0 <= str_start:
            str0 = message[str_start:str_end + 1]
            str0_idx = str_start
            str_end = str_end + 1
        elif 0 <= nul < str_start or str_start < 0 <= nul:
            str0 = message[nul:nul + 4]
            str0_idx = nul
            str_end = nul + 4
        else:
            break

        str_start, str_end, nul = find_string_or_null(message, str_end)

        # get value
        if 0 <= str_start < nul or nul < 0 <= str_start:
            str1 = message[str_start:str_end + 1]
            str1_idx = str_start
            str_end = str_end + 1
        elif 0 <= nul < str_start or str_start < 0 <= nul:
            str1 = message[nul:nul + 4]
            str1_idx = nul
            str_end = nul + 4
        else:
            break

        # append key and value; if value is NULL, take key also as value
        if str1 != 'NULL':
            if str1.lower() in on_offs:
                str_start, str_end, nul = find_string_or_null(message, str_end)
                continue
            res.append(str0)
            res.append(str1)
            msg_ids[str1_idx] = str_end
        elif str0 != 'NULL':
            if str0.lower() in on_offs:
                str_start, str_end, nul = find_string_or_null(message, str_end)
                continue
            res.append(str0)
            res.append(str0)
            msg_ids[str0_idx] = {str1_idx: str_end}
        else:  # no need to include double NULL
            break  # should always break here

        str_start, str_end, nul = find_string_or_null(message, str_end)

    return res, msg_ids


# --------------------          MAIN          -------------------- #
try:
    with open(h_filename, 'r+', encoding='utf-8') as h_file:
        text = h_file.read()
        result = p.findall(text)
        num = len(result)
        seen = set()
        hashes = []
        strings = []
        indeces = []
        for i, msg in enumerate(result):
            data, ids = parse_message(msg)
            indeces.append(ids)
            # setting key
            key = data[0]
            for sp in sp_chars:
                key = key.replace(sp, '')
            key = key.upper().replace(' ', '_')
            # description
            while key.startswith('_'):
                key = key[1:]
            while key.endswith('_'):
                key = key[:-1]
            hashes.append('MSG_HASH_' + key + '_DESC')
            strings.append(data[1])
            # information
            if 1 < len(data[2]):
                for j, alt in enumerate(data[2]):
                    if 2 < len(alt) and alt != 'NULL':
                        hashes.append('MSG_HASH_' + key + '_INFO' + str(j))
                        strings.append(alt)
            else:
                if 2 < len(data[2][0]) and data[2][0] != 'NULL':
                    hashes.append('MSG_HASH_' + key + '_INFO')
                    strings.append(data[2][0])

            # handle the options
            for j in range(3, len(data), 2):
                if data[j + 1] not in seen and not data[j + 1][1:-1].replace('+', '').replace('-', '').isdigit():
                    seen.add(data[j + 1])
                    clean_data = data[j + 1].encode('ascii', errors='ignore').decode('unicode-escape')
                    for sp in sp_chars:
                        clean_data = clean_data.replace(sp, '')
                    clean_data = clean_data.upper().replace(' ', '_')
                    while clean_data.startswith('_'):
                        clean_data = clean_data[1:]
                    while clean_data.endswith('_'):
                        clean_data = clean_data[:-1]
                    hashes.append("MSG_HASH_OPTION_VAL_" + clean_data)
                    strings.append(data[j + 1])

        if not os.path.exists('intl'):
            os.mkdir('intl')

        with open(hash_filename, 'w', encoding='utf-8') as hash_file:
            hash_file.write('#ifndef __MSG_HASH_H\n'
                            '#define __MSG_HASH_H\n\n'
                            '#include <stdint.h>\n'
                            '#include <stddef.h>\n'
                            '#include <limits.h>\n\n'
                            '#define MSG_HASH(Id, str) case Id: return str;\n\n'
                            'enum msg_hash_enums\n'
                            '{\n'
                            '   MSG_UNKNOWN = 0,\n\n')

            for h in hashes:
                hash_file.write('   ' + h + ',\n')
            hash_file.write('   MSG_LAST,\n\n'
                            '   /* Ensure sizeof(enum) == sizeof(int) */\n'
                            '   MSG_DUMMY          = INT_MAX\n'
                            '};\n\n'
                            'static INLINE bool string_is_equal(const char *a, const char *b)\n'
                            '{\n'
                            '   return (a && b) ? !strcmp(a, b) : false;\n'
                            '}\n\n'
                            '/* Callback strings */\n\n'
                            'const char *msg_hash_to_str_ar(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['ARABIC'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_ast(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['ASTURIAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_chs(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['CHINESE_SIMPLIFIED'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n'
                            'const char *msg_hash_to_str_cht(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['CHINESE_TRADITIONAL'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_de(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['GERMAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_el(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['GREEK'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_eo(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['ESPERANTO'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_es(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['SPANISH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_fa(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['PERSIAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_fi(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['FINNISH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_fr(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['FRENCH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_he(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['HEBREW'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_it(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['ITALIAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_jp(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['JAPANESE'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_ko(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['KOREAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_nl(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['DUTCH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_pl(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['POLISH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_pt_br(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['PORTUGUESE_BRAZIL'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_pt_pt(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['PORTUGUESE_PORTUGAL'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_ru(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['RUSSIAN'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_sk(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['SLOVAK'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_tr(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['TURKISH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_us(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['ENGLISH'] +
                            '"\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str_vn(enum msg_hash_enums msg)\n'
                            '{\n'
                            '   switch (msg)\n'
                            '   {\n'
                            '#include "intl/' + intl_files['VIETNAMESE'] +
                            '"\n'
                            '      default:\n'
                            '          break;\n'
                            '   }\n'
                            '   return "null";\n'
                            '}\n\n'
                            'const char *msg_hash_to_str(enum msg_hash_enums msg, unsigned language)\n'
                            '{\n'
                            '   const char *ret = NULL;\n\n'
                            '#ifndef HAVE_NO_LANGEXTRA\n'
                            '   switch (language)\n'
                            '   {\n'
                            '      case RETRO_LANGUAGE_ARABIC:\n'
                            '         ret = msg_hash_to_str_ar(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_ASTURIAN:\n'
                            '         ret = msg_hash_to_str_ast(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_CHINESE_SIMPLIFIED:\n'
                            '         ret = msg_hash_to_str_chs(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_CHINESE_TRADITIONAL:\n'
                            '         ret = msg_hash_to_str_cht(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_DUTCH:\n'
                            '         ret = msg_hash_to_str_nl(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_ESPERANTO:\n'
                            '         ret = msg_hash_to_str_eo(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_FINNISH:\n'
                            '         ret = msg_hash_to_str_fi(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_FRENCH:\n'
                            '         ret = msg_hash_to_str_fr(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_GERMAN:\n'
                            '         ret = msg_hash_to_str_de(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_GREEK:\n'
                            '         ret = msg_hash_to_str_el(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_HEBREW:\n'
                            '         ret = msg_hash_to_str_he(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_ITALIAN:\n'
                            '         ret = msg_hash_to_str_it(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_JAPANESE:\n'
                            '         ret = msg_hash_to_str_jp(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_KOREAN:\n'
                            '         ret = msg_hash_to_str_ko(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_PERSIAN:\n'
                            '         ret = msg_hash_to_str_fa(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_POLISH:\n'
                            '         ret = msg_hash_to_str_pl(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_PORTUGUESE_BRAZIL:\n'
                            '         ret = msg_hash_to_str_pt_br(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_PORTUGUESE_PORTUGAL:\n'
                            '         ret = msg_hash_to_str_pt_pt(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_RUSSIAN:\n'
                            '         ret = msg_hash_to_str_ru(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_SLOVAK:\n'
                            '         ret = msg_hash_to_str_sk(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_SPANISH:\n'
                            '         ret = msg_hash_to_str_es(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_TURKISH:\n'
                            '         ret = msg_hash_to_str_tr(msg);\n'
                            '         break;\n'
                            '      case RETRO_LANGUAGE_VIETNAMESE:\n'
                            '         ret = msg_hash_to_str_vn(msg);\n'
                            '         break;\n'
                            '      default:\n'
                            '         break;\n'
                            '   }\n'
                            '#endif\n\n'
                            '   if (ret && !string_is_equal(ret, "null"))\n'
                            '      return ret;\n\n'
                            '   return msg_hash_to_str_us(msg);\n'
                            '}\n\n'
                            '#endif')

        with open('intl/' + msg_filename, 'w', encoding='utf-8') as string_file:
            string_file.write('#if defined(_MSC_VER) && !defined(_XBOX) && (_MSC_VER >= 1500 && _MSC_VER < 1900)\n'
                              '#if (_MSC_VER >= 1700)\n'
                              '/* https://support.microsoft.com/en-us/kb/980263 */\n'
                              '#pragma execution_character_set("utf-8")\n'
                              '#endif\n'
                              '#pragma warning(disable:4566)\n'
                              '#endif\n\n')
            for idx, s in enumerate(strings):
                string_file.write('MSG_HASH(\n   ' + hashes[idx] + ',\n   ' + s + '\n   )\n')

        with open(code_filename, 'w', encoding='utf-8') as code_file:
            text = text.replace('struct retro_core_option_definition option_defs[] = {',
                                '   size_t   coreOptionSize = 0;\n'
                                '   unsigned language = 0;\n\n'
                                '#ifndef HAVE_NO_LANGEXTRA\n'
                                '   if (!(/* retro_environment_t */(RETRO_ENVIRONMENT_GET_LANGUAGE, &language) &&\n'
                                '        (language < RETRO_LANGUAGE_LAST)))\n'
                                '       language = 0;\n'
                                '#endif\n')

            for m, msg in enumerate(result):
                code = msg
                ids = indeces[m]
                for idx, s in enumerate(strings):
                    instance = code.find(s)
                    while 0 <= instance and instance not in ids:
                        instance = code.find(s, instance + 1)
                    if 0 > instance:  # if s not found
                        continue
                    str_to_insert = 'msg_hash_to_str(' + hashes[idx] + ', language)'
                    length_diff = len(str_to_insert)
                    if not isinstance(ids[instance], dict):
                        instance_end = ids[instance]
                        ids[instance] = instance + length_diff
                        length_diff = length_diff + instance - instance_end
                    else:
                        instance_end = list(ids[instance].values())[0]
                        inner_key = list(ids[instance].keys())[0]
                        ids[instance] = {inner_key: inner_key + length_diff}
                        instance = inner_key
                        length_diff = length_diff + instance - instance_end

                    code = code[:instance] + str_to_insert + code[instance_end:]

                    ids_new = {}
                    for key, value in ids.items():
                        if isinstance(value, dict):
                            inner_key = list(value.keys())[0]
                            if inner_key > instance:
                                value[inner_key + length_diff] = value.pop(inner_key) + length_diff
                                ids_new[key + length_diff] = value
                            else:
                                ids_new[key] = value

                        else:
                            if key > instance:
                                ids_new[key + length_diff] = ids[key] + length_diff  # ids_new.pop(key) + length_diff
                            else:
                                ids_new[key] = value
                    ids = ids_new
                text = text.replace(msg,
                                    'option_defs[coreOptionSize++] = (struct retro_core_option_definition) ' + code[:-1] + ';')
            code_file.write(text.replace('{ NULL, NULL, NULL, {{0}}, NULL },\n};', ''))
    for intl_file in intl_files:
        if intl_file == 'ENGLISH':
            continue
        f = open('intl/' + intl_files[intl_file], 'w')
        f.close()
except EnvironmentError:
    print('Cannot read/write ' + h_filename)
