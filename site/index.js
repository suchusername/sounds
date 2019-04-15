// Author: Nicholas Fazzolari
// Basic tab switching code in pure ES6

// TODO:   Add default tab open feature with an on off switch
//         Make the event listener assignments general

function openTab(tabName) {
    let i;
    let tabContent;
    
    tabContent = document.getElementsByClassName("tab-content");
    
    for (i = 0; i < tabContent.length; i++) {
        tabContent[i].style.display = "none";
    }
    
    document.getElementById(tabName).style.display = "flex";
}


// This needs to DRY'ed up so it can be used with a CMS
let designLinkEl = document.getElementById("DesignLink");
let progLinkEl = document.getElementById("ProgLink");
let musicLinkEl = document.getElementById("SupportLink");

designLinkEl.addEventListener("click", function(){openTab("Design")}, false);
progLinkEl.addEventListener("click", function(){openTab("Programming")}, false);
musicLinkEl.addEventListener("click", function(){openTab("Support")}, false);
