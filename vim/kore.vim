if exists('b:current_syntax')
    finish
endif

let s:cpo_save = &cpo
set cpo&vim

syn keyword koreType        bool byte char enum f32 f64 i8 i16 i32 i64 return str u8 u16 u32 u64 void
syn keyword koreTypedef     type
syn keyword koreConditional if else elseif match
syn keyword koreBoolean     true false
syn keyword koreModule      module
syn keyword koreImport      import
syn keyword koreOptionals   Some None
syn keyword koreRepeat      while for
syn keyword koreFunc        func
syn keyword koreSpecial     export

syn match koreComment "#.*$"
syn match koreNumber "\d\+"

syn region koreString       start=+"+  end=+"+
syn region koreChar         start=+'+  end=+'+
syn region koreMultiComment start="#\*" end="#\*"

hi link koreBoolean      Boolean
hi link koreChar         Character
hi link koreComment      Comment
hi link koreConditional  Conditional
hi link koreFunc         Function
hi link koreImport       Include
hi link koreModule       Include
hi link koreMultiComment Comment
hi link koreNumber       Number
hi link koreOptionals    PreProc
hi link koreRepeat       Repeat
hi link koreSpecial      Special
hi link koreString       String
hi link koreType         Type
hi link koreTypedef      Typedef
