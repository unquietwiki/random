<?php
/* 
   Basic PHP 5.4+ quote generator
   Michael Adams, unquietwiki.com
   Version: 9-3-2016
*/

class Quote{
  
  protected $quoteArray = [
    "How is everyone doing?",
    "cowsay.org has some old-school humor.",
    "Anyone remember Jack Handey quotes?",
    "20+ years ago, this would be some ANSI art.",
    "All this knowledge, but still we're dumb.",
    "Eat local when you travel!",
    "Nothing ever goes as planned.",
    "We all should've be using HTTP compress + IPv6 by now...",
    "This is not Bonzai Buddy."
    ];
  
  
  public function getQuote()
  {    
    return $this->quoteArray[array_rand($this->quoteArray)];
  }
  
}
?>