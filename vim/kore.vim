if exists("b:current_syntax")
    finish
endif

let s:cpo_save = &cpo
set cpo&vim

syn keyword koreType        bool byte enum f32 f64 i8 i16 i32 i64 str u8 u16 u32 u64 void
syn keyword koreTypedef     type
syn keyword koreConditional if else elseif match
syn keyword koreBoolean     true false
syn keyword koreModule      module
syn keyword koreImport      import
syn keyword koreOptionals   Some None
syn keyword koreRepeat      while for
syn keyword koreFunc        func

syn match koreComment "#.*$"

syn region koreString       start=+"+  end=+"+
syn region koreChar         start=+'+  end=+'+
syn region koreMultiComment start="#\*" end="#\*"

hi link koreType         Type
hi link koreTypedef      Typedef
hi link koreConditional  Conditional
hi link koreBoolean      Boolean
hi link koreModule       Include
hi link koreImport       Include
hi link koreOptionals    PreProc
hi link koreRepeat       Repeat
hi link koreFunc         Function
hi link koreString       String
hi link koreChar         Character
hi link koreComment      Comment
hi link koreMultiComment Comment
