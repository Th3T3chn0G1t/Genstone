#### Text

- `%t` - `char*`
- `%tz` - `char*, gen_size_t` (String w/ Length)
- `%c` - `char`
- `%cz` - `char, gen_size_t` (Repeat character size times)

#### Integer Numeric

- Signed
  - `%sz` - `gen_ssize_t`
  - `%sl` - `long`
  - `%si` - `int`
  - `%ss` - `short`
  - `%sc` - `signed char`

- Unsigned
  - `%uz` - `gen_size_t`
  - `%ul` - `unsigned long`
  - `%ui` - `unsigned int`
  - `%us` - `unsigned short`
  - `%uc` - `unsigned char`

#### Floating Point Numeric

- `%fs` - `float`
- `%fd` - `double`
- `%fe` - `long double`

#### Other

- `%p` - `void*`
- `%%` - `void` (`'%'`)
