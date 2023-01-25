
# OMFL Parser

OneMoreFunnyLanguage format

OMFL is a format for storing configuration files. By nature, it is similar to ini, but is more flexible. Its main goal is the ability to get a value from a config by a certain key and logical division into sections and sub-sections.

## Format

OMFL file is a text file in ASCII OMFL format. Further, the format specification will be described. The main construction for OMFL is a KeyValue pair.
The format is case-sensitive. Spaces and empty lines are ignored.

### Key\Value

Key on the left, then the equal sign, then the value.
```text
key = "value"
```

Both key and value are mandatory. Line breaks are prohibited.
Value cannot be overridden for the same key (within one section)

#### Key

Key can consist of:

- Capital and lowercase Latin letters
- Numbers
- Symbols '-' and '_'

The key cannot be zero length.

```text
number = 0
name = "M3100"
```

#### Value

The value can be one of the following types

- integer
- real number
- string
- logical variable
- array of values

#### Integer

Consists of digits (one or more). It is possible to add the character '+' or '-' as the first character.

```text
key1 = 1
key2 = -2022
key3 = +128
```

The possible value will fit in int32_t

#### Real number

Consists of digits (one or more) and one '.'. Before and after the dot, there must be at least one digit
It is possible to add the character '+' or '-' as the first character.

```text
key1 = 1.23
key2 = -2.77
key3 = -0.0001
```

##### String

The string is surrounded by double quotes. Contains any characters.

```text
key1 = "value"
key2 = "Hello world!"
```

#### Boolean value

For boolean values, the literal "true" or "false" is used

```text
key1 = true
key2 = false
```

#### Array

The array is surrounded by the characters '[' and ']'. The elements are separated by ','.
An array can consist of any valid Values, not necessarily of the same type

```text
key1 = [1, 2, 3, 4, 5]
key2 = ["Hello", "world"]
key3 = [[1, 2, 3, 4, 5], ["Hello", "world"]]
key4 = [1, 3.3, "ITMO", [true, false]]
```

### Sections

In addition to the key value block, the format supports sections. Sections allow you to combine sets of Key\Value into logical blocks.
A section is defined as a Key surrounded by the characters '[' and ']'

```text
[name]
```

After declaring a section, all subsequent Key\Value pairs belong to this section, until the next one is declared

```text
[section-1]
key1 = 1
key2 = "2"

[section-2]
key1 = 2
```

Although the section obeys the melting of Keys, it can also contain the symbol '.'. Which defines nested sections.

```text
[section-1.part-1.x]

[section-1.part-1.y]

[section-1.part-2.x]

[section-2.z]
```

Thus, a section can contain both Key\Value pairs and other sections. The key and the name of the subsection cannot match

```text
[A]
key1 = 1

[A.B]
key2 = 3

[A.B.C]
key3 = 3
```

#### Comments

The config may contain a one-line comment. Comments start with the character '#', except when '#' is inside a string value.

```text
    key1 = "value1"  # some comment
    # another comment
```

## Parser

The purpose of the work is to implement the OMFL format parser.

The parser should

- Validate the correctness of the file according to the OMFL format
- Read data from a file into an object whose class has an interface that allows you to get a section or value by key.
- What interface the parser should have is described in the tests

## Tests

The tests directory contains only basic format compliance tests and a parser.
