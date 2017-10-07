function ez_attachEvent(element, evt, func) {
    if (element.addEventListener) {
        element.addEventListener(evt, func, false);
    } else {
        element.attachEvent("on" + evt, func);
    }
}
function ez_attachEventWithCapture(element, evt, func, useCapture) {
    if (element.addEventListener) {
        element.addEventListener(evt, func, useCapture);
    } else {
        element.attachEvent("on" + evt, func);
    }
}
function ez_detachEvent(element, evt, func) {
    if(element.removeEventListener) {
        element.removeEventListener(evt, func);
    } else {
        element.detachEvent("on"+evt, func);
    }
}
function ez_getQueryString(field, url) {
    var href = url ? url : window.location.href;
    var reg = new RegExp('[?&]' + field + '=([^&#]*)', 'i');
    var string = reg.exec(href);
    return string ? string[1] : null;
}/*! EzQueue | Ezoic */
if (typeof execute_ez_queue === "function") {
    ez_attachEvent(window, 'load', execute_ez_queue);
}
/* end: EzQueue */if(typeof ct !== "undefined" && ct !== null) {
    ct.destroy();
}
var ct = {
    DEBUG : false,

    frameTimeoutId : -1,
    frameLoadCount : 0,
    frameElements : [],
    frameData : [],
    currentFrame : null,
    currentFrameIndex : -1,
    stopLoadFrames : false,
    loadFramesTimeoutMs : 800,

    ilLoadIntervalId : -1,
    ilLoadCount : 0,
    stopIlLoad : false,

    oldBrowser : false,

    eventLoopTimeoutId : -1,
    eventLoopRateMs : 100,

    lastActiveElement : null,
    windowHasFocus : false,
    documentHasFocus : false,

    activeFrameIndex : false,
    activeFrame : null,

    twoClickEventTimeoutId : null,

    clickTimeoutMs : 800,

    windowBlurFunc : null,
    windowFocusFunc : null,
    windowBeforeUnloadFunc : null,

    isInitialized : false,

    selectors : [
        [".ezoic_ad > .ezoic_ad", 2],
        [".ez_sa_ad", 2],
        [".ezo_ad > center > .ezoic-ad", 2],
        [".ezoic-ad > .ezoic-ad", 2],
        [".ezo_link_unit_a", 5],
        [".ezo_link_unit_m", 38],
        [".ezo_link_unit_unknown", 0],
        [".ezoic-ad > .OUTBRAIN > .ob-widget", 41],
        [".ezoic-ad > div[id *= 'taboola-'] > .trc_rbox_container", 37],
        [".ezflad", 46],
        [".ezflad-47", 47]
    ],
    init : function() {
        this.log("Init Func called");
        if(this.isInitialized === true) {
            this.log("Initialized already called before.  Not running again.");
            return;
        }
        this.initVars();
        this.loadFrames();

        var self = this;

        this.ilLoadIntervalId = setInterval(function(){self.loadILTrack()}, 500);

        this.startEventLoop();

        this.attachWindowEvents();

        this.isInitialized = true;
    },
    destroy : function() {
        this.log("Destroy Func called");
        this.unloadFrames();
        this.unloadIlTrack();

        this.unsetClickEvents();

        this.stopEventLoop();
        this.detachWindowEvents();
        this.isInitialized = false;
    },
    initVars : function() {
        this.log("Initialize Vars");
        this.frameTimeoutId = -1;
        this.frameLoadCount = 0;
        this.frameElements = [];
        this.frameData = [];
        this.currentFrame = null;
        this.currentFrameIndex = -1;
        this.stopLoadFrames = false;

        this.ilLoadIntervalId = -1;
        this.ilLoadCount = 0;
        this.stopIlLoad = false;

        this.oldBrowser = this.isUndefined(document.hasFocus);

        this.eventLoopTimeoutId = -1;
        this.eventLoopRateMs = 100;

        this.lastActiveElement = null;
        this.windowHasFocus = false;
        this.documentHasFocus = false;

        this.activeFrameIndex = false;
        this.activeFrame = null;

        this.twoClickEventTimeoutId = null;

        this.clickTimeoutMs = 800;

        this.windowBlurFunc = null;
        this.windowFocusFunc = null;
        this.windowBeforeUnloadFunc = null;

        this.isInitialized = false;
    },
    loadFrames : function() {
        this.log("Loading Frames");
        this.frameLoadCount++;
        for(var i = 0; i < this.selectors.length; i++) {
            var elems = document.querySelectorAll(this.selectors[i][0]);
            var statSourceId = this.selectors[i][1];
            for(var j = 0; j < elems.length; j++) {
                this.setClickEvents(elems[j], statSourceId);
            }
        }
        if(this.frameLoadCount > 40) {
            this.stopLoadFrames = true;
        }
        var self = this;
        if (this.stopLoadFrames == false) {
            this.frameTimeoutId = setTimeout(function(){self.loadFrames();}, this.loadFramesTimeoutMs);
        }
    },
    unloadFrames : function() {
        this.log("Unloading frames");
        this.stopLoadFrames = true;

        clearTimeout(this.frameTimeoutId);
    },
    setClickEvents : function(elem, statSourceId) {
        // Return if already set
        if(this.isUndefined(elem.ezo_flag) === false) {
            return;
        }

        this.log("Set Click Events for elem : " + elem.id);

        this.frameElements.push(elem);

        this.frameData.push({
            statSourceId: statSourceId,
            twoClickRecorded: false,
            navigationsRecorded: 0
        });

        var self = this;
        var index = this.frameElements.length - 1;
        elem.ezo_flag = true;
        elem.mouseOverFunc = function() {
            self.log("Mouse Over Func");
            self.currentFrame = this;
            self.currentFrameIndex = index;
        };
        elem.mouseOutFunc = function() {
            self.log("Mouse Out Func");
            self.currentFrame = null;
            self.currentFrameIndex = -1;
        };

        elem.clickFunc = function() {
            self.log("Click Func");
            self.currentFrame = this;
            self.currentFrameIndex = index;
            self.ezAwesomeClick(false, index);
        };

        ez_attachEvent(elem, "mouseover", elem.mouseOverFunc);
        ez_attachEvent(elem, "mouseout", elem.mouseOutFunc);

        if(statSourceId == 46) {
            ez_attachEventWithCapture(elem, "click", elem.clickFunc, true);
        }

        if(statSourceId === 4) {
            elem.mouseOverFuncIl = function() {
                self.log("Mouse Over Il Func");
                if(self.ilLoadCount > 30) {
                    self.ilLoadCount -= 30;
                }
                clearInterval(self.ilLoadIntervalId);

                self.ilLoadIntervalId = setInterval(function(){self.loadILTrack()}, 500);
            };
            ez_attachEvent(elem, "mouseover", elem.mouseOverFuncIl);
        }
        this.log("Finished Set Click Events");
    },
    unsetClickEvents : function() {
        this.log("Unset Click Events");
        while(this.frameElements.length) {
            var elem = this.frameElements.pop();

            if(this.isUndefined(elem) === false) {
                delete elem.ezo_flag;

                ez_detachEvent(elem, "mouseover", elem.mouseOverFunc);
                delete elem.mouseOverFunc;

                ez_detachEvent(elem, "mouseout", elem.mouseOutFunc);
                delete elem.mouseOutFunc;

                if(this.isUndefined(elem.mouseOverFuncIl) === false) {
                    ez_detachEvent(elem, "mouseover", elem.mouseOverFuncIl);
                    delete elem.mouseOverFuncIl;
                }
            }

            this.frameData.pop();
        }
        this.log("Finished unset Click Events");
    },
    loadILTrack : function() {
        this.ilLoadCount++;

        var elems = document.querySelectorAll("span.IL_AD, .IL_BASE");

        for(var i = 0; i < elems.length; i++) {
            var elem = elems[i];
            if(this.isUndefined(elem.ezo_flag) == false) {
                continue;
            }

            if(this.findParentsWithClass(elem, ["IL_AD", "IL_BASE"]) !== false) {
                this.setClickEvents(elem, 4);
            }
        }
        if(this.ilLoadCount > 55) {
            this.log("Il Load Count is over 55.  Stopping.");
            this.stopIlLoad = true;
        }
        if(this.stopIlLoad === true) {
            this.log("Clearing ilLoadInterval");
            clearInterval(this.ilLoadIntervalId);
        }
    },
    unloadIlTrack : function() {
        this.log("Unloading Il Track");
        this.stopIlLoad = true;

        clearInterval(this.ilLoadIntervalId);
    },
    startEventLoop : function() {
        this.log("Starting Event Loop");
        if(this.oldBrowser === true) {
            return;
        }

        var self = this;

        this.eventLoopTimeoutId = setTimeout(function() {self.doEventLoop()}, this.eventLoopRateMs);
    },
    doEventLoop : function() {
        if(this.oldBrowser === true) {
            return;
        }
        var docNowHasFocus = document.hasFocus() && !document.hidden;

        if (this.lastActiveElement !== document.activeElement) {
            if(this.windowHasFocus === false) {
                this.fixedWindowBlur();
            }
            this.lastActiveElement = document.activeElement;
            // If the active element switched, we know the document was momentarily focused on
            this.documentHasFocus = true;
        }

        if(this.documentHasFocus === true && docNowHasFocus === false) {
            this.documentBlur();
        }

        this.documentHasFocus = docNowHasFocus;
        var self = this;

        this.eventLoopTimeoutId = setTimeout(function() {self.doEventLoop()}, this.eventLoopRateMs);
    },
    stopEventLoop : function() {
        this.log("Stopping event loop");
        if(this.oldBrowser === true) {
            return;
        }

        clearTimeout(this.eventLoopTimeoutId);
    },
    documentBlur : function() {
        this.log("Document Blur");
        if(this.twoClickEventTimeoutId !== null) {
            clearTimeout(this.twoClickEventTimeoutId);
        }
        if(this.activeFrameIndex != -1 && this.activeFrameIndex == this.currentFrameIndex) {
            this.ezAwesomeClick(false, this.activeFrameIndex);
        }
    },
    fixedWindowBlur : function() {
        this.log("Fixed Window Blur");
        this.activeFrameIndex = this.searchFrames(document.activeElement);

        if(this.activeFrameIndex < 0) {
            this.activeFrame = null;
            return;
        }

        this.activeFrame = this.frameElements[this.activeFrameIndex];
        var self = this;
        var frameIndex = this.activeFrameIndex;

        this.twoClickEventTimeoutId = setTimeout(function() {
            self.ezAwesomeClick(true, frameIndex);
        }, this.clickTimeoutMs);
    },
    searchFrames : function(frameToFind) {
        for(var i = 0; i < this.frameElements.length; i++) {
            if (this.frameElements[i] === frameToFind || this.frameElements[i].contains(frameToFind)) {
                return i;
            }
        }
        return -1;
    },
    findParentsWithClass : function(elem, classNameList) {
        var parent = elem.parentNode;
        do {
            var classes = parent.className.split(/\s+/);
            for(var i = 0; i < classes.length; i++) {
                for(var j = 0; j < classNameList.length; j++) {
                    if(classes[i] == classNameList[j]) {
                        return parent;
                    }
                }
            }
        } while((parent = parent.parentNode) && this.isUndefined(parent.className) == false);

        return false;
    },
    ezAwesomeClick : function(isTwoClick, frameIndex) {
        this.log("EzAwesomeClick isTwoClick : ", isTwoClick, " and frame index : ", frameIndex);
        this.log(this.frameElements);
        var frameElem = this.frameElements[frameIndex];
        var data = this.frameData[frameIndex];
        var statSourceId = 0;
        if(typeof data != 'undefined') {
            statSourceId = data.statSourceId;
        }

        var adUnitName = this.getAdUnitFromElement(frameElem, statSourceId);

        this.log("adUnitName is: ",adUnitName);

        var paramsObj = null;
        if(adUnitName != "") {
            paramsObj = _ezim_d[adUnitName];
        } else {
            paramsObj = {
                position_id : 0,
                sub_position_id : 0,
                full_id : "0",
                width: 0,
                height: 0
            };
        }

        // For dfp ads, check if this is ox or adsense
        if(statSourceId == 2) {
            var iframes = frameElem.querySelectorAll("iframe");
            if(iframes.length > 0 && iframes[0].id.substring(0,3) == "ox_") {
                statSourceId = 33;
            } else {
                statSourceId = 5;
            }
        }

        if(this.isUndefined(window._ezaq) === true) {
            this.log("_ezaq not defined");
            return;
        }

        // check if clicks have been recorded for this element -- only save one two-click and up to 5 normal clicks
        if(isTwoClick === true) {
            data.twoClickRecorded = true;
        } else {
            // Save to sqs
            document.cookie = "ezoawesome_" + _ezaq.domain_id + "=" + paramsObj.full_id + ' ' + Date.now() + "; path=/;";

            if(data.navigationsRecorded >= 5) {
                return;
            }

            data.navigationsRecorded += 1;
        }

        if(this.isUndefined(window.ezoTemplate) === true ||
            ezoTemplate === "pub_site_noads" ||
            ezoTemplate === "pub_site_mobile_noads" ||
            ezoTemplate === "pub_site_tablet_noads") {
            this.log("no click ezoTemplate is : ", ezoTemplate);
            return;
        }

        if (isTwoClick === false) {
            this.clickRequest("/utilcave_com/awesome.php", {
                url : _ezaq.url,
                width : paramsObj.width,
                height : paramsObj.height,
                did : _ezaq.domain_id,
                sourceid : statSourceId,
                uid : _ezaq.user_id,
                template : ezoTemplate
            });
        }

        this.clickRequest("/ezoic_awesome/", {
            url : _ezaq.url,
            width : paramsObj.width,
            height : paramsObj.height,
            did : _ezaq.domain_id,
            sourceid : statSourceId,
            uid : _ezaq.user_id,
            ff : _ezaq.form_factor_id,
            tid : _ezaq.template_id,
            apid : paramsObj.position_id,
            sapid : paramsObj.sub_position_id,
            iuid : paramsObj.full_id,
            creative : (this.isUndefined(paramsObj.creative_id) === false ? paramsObj.creative_id : ""),
            template : ezoTemplate,
            country : _ezaq.country,
            sub_ad_positions : _ezaq.sub_page_ad_positions,
            twoclick : (isTwoClick === true ? 1 : 0),
            max_ads : _ezaq.max_ads,
            word_count : _ezaq.word_count,
            user_agent : _ezaq.user_agent
        });

        if(isTwoClick === false) {
            this.loadUUIDScript();
        }
    },
    loadUUIDScript : function() {
        if(typeof ezosuigenerisc != "undefined"
            || ((typeof window.isAmp != 'undefined') && isAmp === true)) {
            return;
        }
        this.log("Load UUID Script");
        (function() {
            var el = document.createElement("script");
            el.async = true;
            el.type = 'text/javascript';
            el.src = "//g.ezoic.net/ezosuigenerisc.js";
            var node = document.getElementsByTagName('script')[0];
            node.parentNode.insertBefore(el, node);
        })();
    },
    clickRequest : function(url, data) {
        this.log("Click Request with url : ", url, " and data : ", data);
        if((this.isUndefined(window.ezJsu) === false && ezJsu === true)
            || (this.isUndefined(window._ez_sa) === false && _ez_sa === true)
            || (this.isUndefined(window.isAmp) === false && isAmp == true)){
            url = "//g.ezoic.net" + url;
        } else {
            url = window.location.protocol + "//" + window.location.host + url;
        }

        var request = new XMLHttpRequest();
        var request_type = true;

        // Make request async on desktop and synchronous on mobile/tablet
        if(this.isMobileOperatingSystem() === true ) {
            request_type = false;
        }

        request.open('POST', url, request_type);
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
        var queryData = [];
        for(var param in data) {
            queryData.push(param + "=" + encodeURIComponent(data[param]));
        }
        request.send(queryData.join("&"));
    },
    getAdUnitFromElement : function(elem, statSourceId) {
        if(this.isUndefined(window._ezim_d) === true) {
            this.log("_ezim_d not found");
            return "";
        }

        if(statSourceId == 4) {
            for(key in _ezim_d) {
                if(key.indexOf("inline-1") != -1) {
                    this.log("found inline");
                    return key;
                }
            }
        } else if (statSourceId == 37 || statSourceId == 41) {
            var widgetWrapParent = this.findParentsWithClass(elem, ["ezoic-ad"]);
            if(widgetWrapParent !== false) {
                var adId = widgetWrapParent.className.replace("ezoic-ad", "").replace(/^\s+|\s+$/g, '');

                for (var key in _ezim_d) {
                    if(key.indexOf(adId) != -1) {
                        this.log("found native");
                        return key;
                    }
                }
            }
        } else if (this.isUndefined(elem.adunitname) === false) {
            this.log("found ad unit from elem.adunitname field");
            return elem.adunitname;
        } else if (elem.getAttribute('adunitname') != null) {
            this.log("found ad unit from property field: ",elem.getAttribute('adunitname'));
            return elem.getAttribute('adunitname');
        } else {
            for (key in _ezim_d) {
                if(elem.id.indexOf(key) != -1) {
                    this.log("found on _ezim_d");
                    return key;
                }
            }
        }

        return "";
    },
    attachWindowEvents : function() {
        this.log("Attaching window events");
        var self = this;
        this.windowBlurFunc = function() {
            self.log("Window Blur Func");
            self.windowHasFocus = false;

            if(self.lastActiveElement !== document.activeElement && self.oldBrowser === false) {
                self.fixedWindowBlur();
                self.lastActiveElement = document.activeElement;
            } else if (self.currentFrame !== null) {
                self.ezAwesomeClick(false, self.currentFrameIndex);
            }
        };

        this.windowFocusFunc = function() {
            self.log("Window Focus Func");
            self.windowHasFocus = true;
            self.activeFrame = null;
            self.activeFrameIndex = -1;
        };

        this.windowBeforeUnloadFunc = function() {
            self.log("Window Before Unload Func");
            if(self.twoClickEventTimeoutId !== null) {
                clearTimeout(self.twoClickEventTimeoutId);
            }

            // We don't have some events being called
            // We can account for that here
            if( self.isMobileOperatingSystem() ) {
                self.fixedWindowBlur();
            }

            if(self.currentFrameIndex != -1
                && self.activeFrameIndex == self.currentFrameIndex
                && self.frameData[self.activeFrameIndex].navigationsRecorded == 0) {
                self.ezAwesomeClick(false, self.activeFrameIndex);
            }
        };

        ez_attachEvent(window, "blur", this.windowBlurFunc);
        ez_attachEvent(window, "focus", this.windowFocusFunc);
        ez_attachEvent(window, "beforeunload", this.windowBeforeUnloadFunc);

        if(this.isIosUserAgent() === true) {
            this.log("Attaching pagehide");
            ez_attachEvent(window, "pagehide", this.windowBeforeUnloadFunc);
        }
    },
    detachWindowEvents : function() {
        this.log("Detaching window events.");
        ez_detachEvent(window, "blur", this.windowBlurFunc);
        ez_detachEvent(window, "focus", this.windowFocusFunc);
        ez_detachEvent(window, "beforeunload", this.windowBeforeUnloadFunc);

        if(this.isIosUserAgent() === true) {
            ez_detachEvent(window, "pagehide", this.windowBeforeUnloadFunc);
        }
    },
    isUndefined : function() {
        for (var i = 0; i < arguments.length; i++) {
            if (typeof arguments[i] === 'undefined' || arguments[i] === null) {
                return true;
            }
        }
        return false;
    },
    log : function() {
        if(this.DEBUG) {
            console.log.apply(console, arguments);
        }
    },
    isMobileOperatingSystem : function() {
        return typeof ezoFormfactor !== "undefined" && (ezoFormfactor == "2" || ezoFormfactor == "3");
    },
    isIosUserAgent : function() {
        return navigator.userAgent.indexOf("iPad") != -1 ||
            navigator.userAgent.indexOf("iPhone") != -1 ||
            navigator.userAgent.indexOf("iPod") != -1;
    }
};
ct.init();
var ezdent=ezdent||{};ezdent.msgs=[];ezdent.debug=function(){if(!ezDenty.processed){return;}if(ezdent.msgs.length>0){for(var ll=0,imax=ezdent.msgs.length;ll<imax;ll++){console.debug(ezdent.msgs[ll]);}}ezDenty.highlight();};ezdent.log=function(l1){ezdent.msgs.push(l1);};ezdent.Denty=function(){this.headTag=document.getElementsByTagName('head').item(0);this.stylesheet='';this.displayQ=['ins.adsbygoogle','iframe[id^="_mN_main_"]','ins[id^="aswift_"] > iframe','iframe.switch_request_frame'];this.nativeQ=['.OUTBRAIN > .ob-widget','div[id*="taboola-"] > .trc_rbox_container','div.rc-wc.rc-bp'];this.initArrays();this.processed=false;};ezdent.Denty.prototype.Process=function(){this.setSizes();this.getDisplay();this.getNative();this.fire();this.processed=true;ezdent.log(this);};ezdent.Denty.prototype.addA=function(el,type){if(typeof el==="undefined"||el===null){return;}if(!this.alreadyFound(el,5)&&el.clientWidth>0&&el.clientHeight>0){this.as.push(new ezdent.Item(el,type));}};ezdent.Denty.prototype.alreadyFound=function(el,numElsToCheck){if(typeof el.parentNode!=="undefined"){var parent=el.parentNode;for(var ll=0,imax=numElsToCheck;ll<imax;ll++){if(typeof parent!=="undefined"&&parent!=null&&typeof parent.hasAttribute=="function"&&parent.hasAttribute("class")&&parent.classList.contains('ezfound')){return true;}parent=parent.parentNode;if(typeof parent==="undefined"||parent==null){break;}}}var lI=el.querySelector('.ezfound');return lI!=null;};ezdent.Denty.prototype.destroy=function(){if(this.stylesheet!=='')this.headTag.removeChild(this.stylesheet);this.removeClasses();this.initArrays();};ezdent.Denty.prototype.fire=function(){if(typeof _ezaq==="undefined"||!_ezaq.hasOwnProperty("page_view_id")){return;}var l1l=_ezaq["page_view_id"],p=this.getPD(),pxArr=[];if(typeof p==="object"&&Object.keys(p).length>0){for(var l11 in p){if(p.hasOwnProperty(l11)===false)continue;pxArr.push(new __ezDotData(l11,p[l11]));}__ez.bit.Add(l1l,pxArr);__ez.bit.Fire();ezdent.log(p);}};ezdent.Denty.prototype.getDisplay=function(){this.getDisplayDfp();if(this.displayQ.length<1){return;}for(var ll=0,imax=this.displayQ.length;ll<imax;ll++){var els=document.querySelectorAll(this.displayQ[ll]);if(els.length>0){for(var l1I=0,jmax=els.length;l1I<jmax;l1I++){this.addA(els[l1I],'display');}}}};ezdent.Denty.prototype.getDisplayDfp=function(){if(typeof googletag==='undefined'||googletag===null||typeof googletag.pubads!=='function'){return;}var slots=googletag.pubads().getSlots();for(var ll=0,imax=slots.length;ll<imax;ll++){var lIl=slots[ll].getSlotElementId(),slotEl=document.getElementById(lIl);if(typeof slotEl!=='undefined'){this.addA(slotEl,'display');}}};ezdent.Denty.prototype.getNative=function(){if(this.nativeQ.length<1){return;}for(var ll=0,imax=this.nativeQ.length;ll<imax;ll++){var els=document.querySelectorAll(this.nativeQ[ll]);if(els.length>0){for(var l1I=0,jmax=els.length;l1I<jmax;l1I++){this.addA(els[l1I],'native');}}}};ezdent.Denty.prototype.getPD=function(){var p=[];p["display_ad_viewport_px"]=0;p["display_ad_viewport_count"]=0;p["native_ad_viewport_px"]=0;p["native_ad_viewport_count"]=0;p["display_ad_doc_px"]=0;p["display_ad_doc_count"]=0;p["native_ad_doc_px"]=0;p["native_ad_doc_count"]=0;p["viewport_size"]=this.viewportSize[0]+"x"+this.viewportSize[1];p["viewport_px"]=this.viewportSize[0]*this.viewportSize[1];p["doc_px"]=this.documentSize[0]*this.documentSize[1];p["doc_height"]=this.documentSize[1];if(this.as.length<1){return p;}for(var ll=0,imax=this.as.length;ll<imax;ll++){var a=this.as[ll];if(a.onScreen){if(this.isBF(a.el,3)==false){p[a.type+"_ad_viewport_px"]=p[a.type+"_ad_viewport_px"]+a.getPxInView();}else{ezdent.log("BF not adding");}p[a.type+"_ad_viewport_count"]++;}p[a.type+"_ad_doc_px"]+=a.height*a.width;p[a.type+"_ad_doc_count"]++;}return p;};ezdent.Denty.prototype.highlight=function(){this.stylesheet=document.createElement("style");this.stylesheet.innerHTML=".ezhlght-on{border:5px solid aqua!important;}.ezhlght-off{border:5px solid red!important;}";this.headTag.insertBefore(this.stylesheet,this.headTag.firstChild);if(this.as.length>0){for(var ll=0,imax=this.as.length;ll<imax;ll++){if(this.as[ll].onScreen){this.as[ll].el.classList.add("ezhlght-on");}else{this.as[ll].el.classList.add("ezhlght-off");}}}};ezdent.Denty.prototype.initArrays=function(){this.as=[];this.viewportSize=[];this.windowSize=[];this.documentSize=[];};ezdent.Denty.prototype.isBF=function(el,numElsToCheck){if(typeof el.hasAttribute=="function"&&el.hasAttribute("class")&&el.classList.contains("ezoic-floating-bottom")){return true;}if(typeof el.parentNode!=="undefined"){var parent=el.parentNode;for(var ll=0,imax=numElsToCheck;ll<imax;ll++){if(typeof parent!=="undefined"&&parent!=null&&typeof parent.hasAttribute=="function"&&parent.hasAttribute("class")&&parent.classList.contains("ezoic-floating-bottom")){return true;}parent=parent.parentNode;if(typeof parent==="undefined"||parent==null){break;}}}var lI1=el.querySelector('.ezoic-floating-bottom');return lI1!=null;};ezdent.Denty.prototype.removeClasses=function(){if(this.as.length>0){for(var ll=0,imax=this.as.length;ll<imax;ll++){this.as[ll].el.classList.remove("ezhlght-on");this.as[ll].el.classList.remove("ezhlght-off");this.as[ll].el.classList.remove("ezfound");}}};ezdent.Denty.prototype.setSizes=function(){var body=document.body,html=document.documentElement;var lII=window.innerWidth||document.documentElement.clientWidth||document.body.clientWidth;var vpH=window.innerHeight||document.documentElement.clientHeight||document.body.clientHeight;lII=Math.min(lII,10000);vpH=Math.min(vpH,10000);this.viewportSize=[lII,vpH];this.documentSize=[Math.max(body.scrollWidth,body.offsetWidth,html.clientWidth,html.scrollWidth,html.offsetWidth),Math.max(body.scrollHeight,body.offsetHeight,html.clientHeight,html.scrollHeight,html.offsetHeight)];};ezdent.Item=function(el,type){this.el=el;this.type=type;this.width=el.clientWidth;this.height=el.clientHeight;this.coords=this.getCoords();this.onScreen=this.ios();if(typeof el.classList!='undefined'){el.classList.add("ezfound");}};ezdent.Item.prototype.getCoords=function(){var box=this.el.getBoundingClientRect();var body=document.body;var docEl=document.documentElement;var scrollTop=window.pageYOffset||docEl.scrollTop||body.scrollTop;var scrollLeft=window.pageXOffset||docEl.scrollLeft||body.scrollLeft;var clientTop=docEl.clientTop||body.clientTop||0;var clientLeft=docEl.clientLeft||body.clientLeft||0;var top=box.top+scrollTop-clientTop;var left=box.left+scrollLeft-clientLeft;return{top:Math.round(top),left:Math.round(left)};};ezdent.Item.prototype.getPxInView=function(){var l1ll=this.height;if((this.coords.top+this.height)>window.innerHeight){l1ll=window.innerHeight-this.coords.top;}var l1l1=this.width;if((this.coords.left+this.width)>window.innerWidth){l1l1=window.innerWidth-this.coords.left;}ezdent.log(this.el);ezdent.log('usable '+l1ll+' * '+l1l1);ezdent.log(l1l1*l1ll);return l1l1*l1ll;};ezdent.Item.prototype.ios=function(){return(this.coords.top<=window.innerHeight&&this.coords.left>=0&&this.coords.left<=window.innerWidth);};var ezDenty=new ezdent.Denty();setTimeout(function(){ezDenty.Process();},7500);