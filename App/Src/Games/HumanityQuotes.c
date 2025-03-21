
#include "Games/Humanity.h"

char blacks[NUM_BLACKS][4][21] = {
    {
        "_______? No.",
    },
    {
        "On the 12th day of C",
        "hristmas, my true lo",
        "ve gave to me: _____",
    },
    {
        "All I want for Chris",
        "tmas is _______ .",
    },
    {
        "Alright, kids. The v",
        "otes are in, and the",
        "new school mascot w",
        "ill be ______.",
    },
    {
        "Alright, which one o",
        "f you little turds i",
        "s responsible for __",
        "____?!",
    },
    {
        "And over here is Pic",
        "asso's famous painti",
        "ng, \"Portrait of __",
    },
    {
        "At school, I'm just",
        "Mandy. But at summer",
        "camp, I'm \"______",
        "Mandy.\"",
    },
    {
        "Beep beep! ______ co",
        "ming through!",
    },
    {
        "Bow before me, for I",
        "am the Queen of ___",
        "___!",
    },
    {
        "\"Boys? No.",
    },
    {
        "______? Yes!\"",
    },
    {
        "Charmander has evolv",
        "ed into _______!",
    },
    {
        "\"CHUNGO ANGRY.",
    },
    {
        "CHUNGO",
    },
    {
        "DESTROY ______.\"",
    },
    {
        "CHUNGO FEEL SICK. CH",
        "UNGO NO LIKE ______",
        "ANYMORE.",
    },
    {
        "Class, pay close att",
        "ention. I will now d",
        "emonstrate the physi",
        "cs of ______.",
    },
    {
        "CNN breaking news! O",
        "ver half of American",
        "s are now _______.",
    },
    {
        "Come on, Danny. All",
        "the cool kids are do",
        "in' it. Wanna try __",
        "_____?",
    },
    {
        "Come with me, and I",
        "will show you a worl",
        "d of ______.",
    },
    {
        "Coming soon! Batman",
        "vs. ______.",
    },
    {
        "Coming soon! \"Batma",
        "n vs. ______.\"",
    },
    {
        "Coming to theaters t",
        "his holiday season,",
        "\"Star Wars: The Ris",
        "e of ______.\"",
    },
    {
        "Did you hear about t",
        "he new Avenger? She'",
        "s ______!",
    },
    {
        "Did you know that Be",
        "njamin Franklin inve",
        "nted ______?",
    },
    {
        "Disney proudly prese",
        "nts \"_______ in Ice",
        ".\"",
    },
    {
        "ENOUGH! I will not l",
        "et ______ tear this",
        "family apart!",
    },
    {
        "Ew. Grandpa smells l",
        "ike _______.",
    },
    {
        "Foolish child! Did y",
        "ou think you could e",
        "scape from ______?",
    },
    {
        "Girls just wanna hav",
        "e ______.",
    },
    {
        "Guys, stop it! There",
        "'s nothing funny abo",
        "ut ______.",
    },
    {
        "Hey Riley, I'll give",
        "you five bucks if y",
        "ou try _______.",
    },
    {
        "Hey, check out my ba",
        "nd! We're called \"R",
        "age Against ______.\"",
    },
    {
        "\"I do not fight for",
        "wealth.",
    },
    {
        "I do not fight for g",
        "lory.",
    },
    {
        "I fight for _______!",
        "\"",
    },
    {
        "I have invented a ne",
        "w sport. I call it \"",
        "______ ball.\"",
    },
    {
        "I lost my arm in a _",
        "_____ accident.",
    },
    {
        "I'm sorry, Mrs. Sanc",
        "hez, but I couldn't",
        "finish my homework b",
        "ecause of ______.",
    },
    {
        "Isn't this great, ho",
        "ney? Just you, me, t",
        "he kids, and _______",
        ".",
    },
    {
        "\"It's BIG.",
    },
    {
        "It's SCARY.",
    },
    {
        "It's _______!\"",
    },
    {
        "James Bond will retu",
        "rn in \"No Time for",
        "_______.\"",
    },
    {
        "Joey, look out behin",
        "d you! It's ______!",
    },
    {
        "Kids, Dad is trying",
        "something new this w",
        "eek. It's called \"_",
        "_____\".",
    },
    {
        "\"Little Miss Muffet",
        "",
    },
    {
        "Sat on a tuffet",
    },
    {
        "Eating her curds and",
        "______.\"",
    },
    {
        "Madam President, we'",
        "ve run out of time.",
        "The only option is _",
        "_______.",
    },
    {
        "Me and my friends do",
        "n't play with dolls",
        "anymore. We're into",
        "______ now.",
    },
    {
        "ME HUNGRY. ME WANT _",
        "______.",
    },
    {
        "Mom? You have to com",
        "e pick me up! There'",
        "s ______ at this par",
        "ty.",
    },
    {
        "Mom!? You have to co",
        "me pick me up! There",
        "'s ______ at this pa",
        "rty.",
    },
    {
        "Moms love ______.",
    },
    {
        "My dad and I enjoy _",
        "_____ together.",
    },
    {
        "My favorite book is",
        "\"The Amazing Advent",
        "ures of ______.\"",
    },
    {
        "My favorite book is",
        "The Amazing Adventur",
        "es of ______.",
    },
    {
        "My favorite dinosaur",
        "is \"______asaurus.",
        "\"",
    },
    {
        "MY NAME CHUNGO. CHUN",
        "GO LOVE ______.",
    },
    {
        "Never fear, Captain",
        "______ is here!",
    },
    {
        "New from Hasbro! It'",
        "s BUNGO: The Game of",
        "______.",
    },
    {
        "New from Mattel! It'",
        "s BUNGO: The Game of",
        "______.",
    },
    {
        "New from Mattel, it'",
        "s ______ Barbie!",
    },
    {
        "New from McDonalds:",
        "it's the Mc______ Bu",
        "rger.",
    },
    {
        "Next from J.K. Rowli",
        "ng: Harry Potter and",
        "the Chamber of ____",
        "__.",
    },
    {
        "Next from J.K. Rowli",
        "ng: Harry Potter and",
        "the Chamber of ____",
        "__.",
    },
    {
        "No fair! How come Ch",
        "loe gets her own pho",
        "ne, and allI get is",
        "______?",
    },
    {
        "Now in bookstores: N",
        "ancy Drew and the My",
        "stery of ______.",
    },
    {
        "Now in bookstores: N",
        "ancy Drew and the My",
        "stery of ______.",
    },
    {
        "Oh Dark Lord, we sho",
        "w our devotion with",
        "a humble offering of",
        "______!",
    },
    {
        "Oh, no thank you, Mr",
        "s. Lee. I've had ple",
        "nty of ______ for no",
        "w.",
    },
    {
        "Oh, that's my mom's",
        "friend Carl. he come",
        "s over and helps her",
        "with _______.",
    },
    {
        "On the next episode",
        "of Dora the Explorer",
        ", Dora explorers ___",
        "____.",
    },
    {
        "\"One nation,",
    },
    {
        "under God,",
    },
    {
        "indivisible,",
    },
    {
        "with liberty and",
    },
    {
        "______",
    },
    {
        "for all.\"",
    },
};

char whites[NUM_WHITES][4][21] = {
    {
        "\"Baby Shark\" on re",
        "peat forever.",
    },
    {
        "14 cheeseburgers, 6",
        "large fries, and a m",
        "edium Sprite.",
    },
    {
        "A 40-piece Chicken M",
        "cNuggets.",
    },
    {
        "A baby boomer.",
    },
    {
        "A baby with a full m",
        "ustache.",
    },
    {
        "A balloon filled wit",
        "h chili.",
    },
    {
        "A bear.",
    },
    {
        "A big rock.",
    },
    {
        "A big sad dragon wit",
        "h no friends.",
    },
    {
        "A big wet kiss from",
        "Great Aunt Sharon.",
    },
    {
        "A big whiny cry-baby",
        ".",
    },
    {
        "A big, and I mean BI",
        "G turtle.",
    },
    {
        "A big, fat, wiggly,",
        "squishy, talking, sc",
        "reaming turd.",
    },
    {
        "A bird pooping on th",
        "e president's head.",
    },
    {
        "A black hole.",
    },
    {
        "A bountiful harvest",
        "of squashes and corn",
        "s.",
    },
    {
        "A bra.",
    },
    {
        "A bunch of dead squi",
        "rrels on a trampolin",
        "e.",
    },
    {
        "A burrito smoothie.",
    },
    {
        "A butt that eats und",
        "erwear.",
    },
    {
        "A caverperson named",
        "Helen.",
    },
    {
        "A chicken that has p",
        "ickles as horns and",
        "a butt the size of a",
        "bus.",
    },
    {
        "A cloud that rains d",
        "iarrhea.",
    },
    {
        "A corn dog.",
    },
    {
        "A couch that eats ch",
        "ildren.",
    },
    {
        "A cow.",
    },
    {
        "A cowboy who is half",
        "boy, half cow.",
    },
    {
        "A cursed llama with",
        "no eyes.",
    },
    {
        "A dead body.",
    },
    {
        "A dead whale.",
    },
    {
        "A Democrat.",
    },
    {
        "A dinosaur that is p",
        "ooping but doesn't k",
        "now it is pooping.",
    },
    {
        "A dog that stares in",
        "to eternal nothingne",
        "ss.",
    },
    {
        "A doll that pees rea",
        "l pee!",
    },
    {
        "A fake kid made out",
        "of wood.",
    },
    {
        "A flamethrower.",
    },
    {
        "A gerbil named \"Ger",
        "bil.\"",
    },
    {
        "A glorious beard.",
    },
    {
        "A horse with no legs",
        ".",
    },
    {
        "A hot air balloon po",
        "wered by fart gas.",
    },
    {
        "A hug.",
    },
    {
        "A huge honkin' carro",
        "t.",
    },
    {
        "A hundred screaming",
        "monkeys.",
    },
    {
        "A killer clown.",
    },
    {
        "A long, hot pee.",
    },
    {
        "A long, long snake.",
    },
    {
        "A naked lady in a pa",
        "inting.",
    },
    {
        "A nice, warm glass o",
        "f pee.",
    },
    {
        "A pirate with two pe",
        "g arms, two peg legs",
        ", and a peg head.",
    },
    {
        "A plant-devouring sp",
        "ace-worm named \"Rac",
        "hel.\"",
    },
    {
        "A Pokémon Named \"J",
        "im.\"",
    },
    {
        "A poop as big as Mom",
        ".",
    },
    {
        "A pregnant person.",
    },
    {
        "A Pringle.",
    },
    {
        "A Republican.",
    },
    {
        "A scoop of tuna.",
    },
    {
        "A screaming soccer d",
        "ad.",
    },
    {
        "A statue of a naked",
        "guy.",
    },
    {
        "A super angry cat I",
        "found outside.",
    },
    {
        "A tiny detective who",
        "solves tiny crimes.",
        "",
    },
    {
        "A truck.",
    },
    {
        "A turd that just won",
        "'t flush.",
    },
    {
        "A whole thing of cot",
        "tage cheese.",
    },
    {
        "A wise, old woman wi",
        "th no teeth and clou",
        "dy eyes.",
    },
    {
        "Abraham Lincoln.",
    },
    {
        "Acting kinda sus.",
    },
    {
        "Aliens.",
    },
    {
        "All of your teeth fa",
        "lling out.",
    },
    {
        "An eggy smell.",
    },
    {
        "An invisible giant w",
        "ho takes giant, visi",
        "ble poops.",
    },
    {
        "An old, dirty cat wi",
        "th bad breath.",
    },
    {
        "An order of mozzarel",
        "la sticks.",
    },
    {
        "An owl that hates yo",
        "u.",
    },
    {
        "Anime.",
    },
    {
        "Anti-vaxxers.",
    },
    {
        "Ariana Grande.",
    },
    {
        "Athena, Goddess of W",
        "isdom.",
    },
    {
        "Aunt Linda.",
    },
    {
        "Baby boomers.",
    },
    {
        "Baby Yoda.",
    },
    {
        "Bad parenting.",
    },
    {
        "Balls.",
    },
    {
        "Barf.",
    },
    {
        "Barfing into a popco",
        "rn bucket.",
    },
    {
        "Batman.",
    },
    {
        "Beautiful Grandma.",
    },
    {
        "Becoming taller and",
        "stronger than Father",
        ".",
    },
    {
        "Becoming the preside",
        "nt.",
    },
    {
        "Beer.",
    },
    {
        "Being a dinosaur.",
    },
    {
        "Being adopted.",
    },
    {
        "Being dead.",
    },
    {
        "Being fake.",
    },
    {
        "Being famous on YouT",
        "ube.",
    },
    {
        "Being French, hoh-ho",
        "h-hoh!",
    },
    {
        "Being super serious",
        "right now.",
    },
    {
        "Bench pressing a hor",
        "se.",
    },
    {
        "Beyoncé.",
    },
    {
        "Big butt cheeks fill",
        "ed with poop.",
    },
    {
        "Big Mississippi boys",
        ".",
    }
};