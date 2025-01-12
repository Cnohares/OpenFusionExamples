//----------------------------------------------------------------------------------
//
// CRunFindTheWay.js
//
//----------------------------------------------------------------------------------

// Definition of the conditions, actions and expressions codes.
// ---------------------------------------------------------------
// This list must be identical to the list defined in the C version
// of your extension.
CRunFindTheWay.CND_CONDITION_SMBS = 0;
CRunFindTheWay.CND_CONDITION_SMBB = 1;
CRunFindTheWay.CND_CONDITION_SMBC = 2;
CRunFindTheWay.CND_CONDITION_OSMBC = 3;

CRunFindTheWay.CND_CONDITION_OPF = 4;
CRunFindTheWay.CND_CONDITION_PA = 5;

CRunFindTheWay.CND_CONDITION_OITP = 6;

CRunFindTheWay.CND_CONDITION_MA = 7;
CRunFindTheWay.CND_CONDITION_CMCAC = 8;

CRunFindTheWay.CND_CONDITION_OAG = 9;
CRunFindTheWay.CND_CONDITION_OAO = 10;
CRunFindTheWay.CND_CONDITION_OAC = 11;

CRunFindTheWay.CND_CONDITION_SMBP = 12;

CRunFindTheWay.CND_CONDITION_CA = 13;
CRunFindTheWay.CND_CONDITION_OITA = 14;
CRunFindTheWay.CND_CONDITION_OITAA = 15;
CRunFindTheWay.CND_CONDITION_AITA = 16;

CRunFindTheWay.CND_CONDITION_OMC = 17;

CRunFindTheWay.CND_CONDITION_OCOZ = 18;
CRunFindTheWay.CND_CONDITION_ZV = 19;
CRunFindTheWay.CND_CONDITION_ZAA = 20;
CRunFindTheWay.CND_CONDITION_SA = 21;
CRunFindTheWay.CND_CONDITION_OAOBJ = 22;
CRunFindTheWay.CND_CONDITION_POAO = 23;

CRunFindTheWay.CND_CONDITION_NOAG = 24;
CRunFindTheWay.CND_CONDITION_NOAC = 25;

CRunFindTheWay.CND_CONDITION_SMBA = 26;
CRunFindTheWay.CND_CONDITION_SMBSF = 27;

// Important. CND_LAST must contain the number of conditions. 
// Do not forget to update it if you add or remove a condition from the list.
CRunFindTheWay.CND_LAST = 28;

CRunFindTheWay.ACT_ACTION_SM = 0;
CRunFindTheWay.ACT_ACTION_C = 1;
CRunFindTheWay.ACT_ACTION_ITP = 2;
CRunFindTheWay.ACT_ACTION_SMBO = 3;
CRunFindTheWay.ACT_ACTION_CM = 4;
CRunFindTheWay.ACT_ACTION_SZBO = 5;
CRunFindTheWay.ACT_ACTION_CZ = 6;
CRunFindTheWay.ACT_ACTION_ITA = 7;
CRunFindTheWay.ACT_ACTION_SUBO = 8;
CRunFindTheWay.ACT_ACTION_CU = 9;
CRunFindTheWay.ACT_ACTION_COZ = 10;
CRunFindTheWay.ACT_ACTION_SS = 11;
CRunFindTheWay.ACT_ACTION_COZBE = 12;
CRunFindTheWay.ACT_ACTION_COZBN = 13;
CRunFindTheWay.ACT_ACTION_CAO = 14;
CRunFindTheWay.ACT_ACTION_CABNO = 15;
CRunFindTheWay.ACT_ACTION_CAOE = 16;
CRunFindTheWay.ACT_ACTION_CAOEBN = 17;
CRunFindTheWay.ACT_ACTION_CG = 18;
CRunFindTheWay.ACT_ACTION_SGS = 19;

CRunFindTheWay.EXP_EXPRESSION_GITX = 0;
CRunFindTheWay.EXP_EXPRESSION_GITY = 1;

CRunFindTheWay.EXP_EXPRESSION_GS = 2;
CRunFindTheWay.EXP_EXPRESSION_GSOP = 3;
CRunFindTheWay.EXP_EXPRESSION_GSCOP = 4;

CRunFindTheWay.EXP_EXPRESSION_GGC = 5;
CRunFindTheWay.EXP_EXPRESSION_GRC = 6;

CRunFindTheWay.EXP_EXPRESSION_GITI = 7;

CRunFindTheWay.EXP_EXPRESSION_GMC = 8;
CRunFindTheWay.EXP_EXPRESSION_GMB64 = 9;
CRunFindTheWay.EXP_EXPRESSION_GMS = 10;

CRunFindTheWay.EXP_EXPRESSION_GALR = 11;

CRunFindTheWay.EXP_EXPRESSION_GIF = 12;

CRunFindTheWay.EXP_EXPRESSION_GMTM = 13;
CRunFindTheWay.EXP_EXPRESSION_GMTT = 14;
CRunFindTheWay.EXP_EXPRESSION_GMTD = 15;
CRunFindTheWay.EXP_EXPRESSION_GMCP = 16;
CRunFindTheWay.EXP_EXPRESSION_GMCO = 17;
CRunFindTheWay.EXP_EXPRESSION_GMCV = 18;

CRunFindTheWay.EXP_EXPRESSION_GIGS = 19;

// Constructor of the object.
// ----------------------------------------------------------------
// Called during the creation process of the object, but before any 
// initialization. You may want (although you can do it in CreateRunObject), 
// to instantiate variables.
function CRunFindTheWay() {
    // this.myVariable = 0;
    // this.anObject = null;
}

// Prototype definition
// -----------------------------------------------------------------
// This class is a sub-class of CRunExtension, by the mean of the 
// CServices.extend function which copies all the properties of 
// the parent class to the new class when it is created.
// As all the necessary functions are defined in the parent class,
// you only need to keep the ones that you actually need in your code.
CRunFindTheWay.prototype = CServices.extend(new CRunExtension(),
    {
        // Returns the number of conditions
        // --------------------------------------------------------------------
        // Warning, if this number is not correct, the application _will_ crash
        getNumberOfConditions: function () {
            return CRunFindTheWay.CND_LAST;
        },                                              // Don't forget the comma between each function

        // Creation of the object
        // --------------------------------------------------------------------
        // - file : a CFile object, pointing to the object's data zone
        // - cob : a CCreateObjectInfo containing infos about the created object
        // - version : the version number of the object, as defined in the C code
        createRunObject: function (file, cob, version) {
            // Use the "file" parameter to call the CFile object, and 
            // gather the data of the object in the order as they were saved
            // (same order as the definition of the data in the EDITDATA structure
            // of the C code).
            // Examples :
            // this.myIntegerValue = file.readAInt();   Reads 4 bytes as a signed number
            // this.myShortValue = file.readAShort();   Reads 2 bytes as a unsigned number
            // this.myString = file.readAString();      Reads a string, ending by 0
            // this.myString = file.readAString(size);  Reads a string out of a given sized buffer
            //                                            - The string can end before the end of the buffer with a 0
            //                                            - If the string is as long as the buffer, it does not 
            //                                              need to end by a 0
            //                                            > Whatever happens, this function will always position the file
            //                                              at the end of the buffer upon exit
            // this.myString = file.readAStringEOL();   Reads a string until a CR or a CR/LF (works with both)
            // this.myColor = file.readAColor();        Reads a RGB value
            // file.skipBytes(number);                  Skips a number of bytes in the file
            // Please report to the documentation for more information on the CFile object

            // The return value is not used in this version of the runtime but maybe later.
            // So please return false;
            return false;
        },

        // Handling of the object
        // ---------------------------------------------------------------------
        // This function is called at every loop of the game. You have to perform 
        // in it all the tasks necessary for your object to function.
        // Return values:
        //    - 0 : this function will be called during the next loop
        //    - CRunExtension.REFLAG_ONESHOT : this function will not be called anymore
        //      In this case, call the this.reHandle(); function of the base class 
        //      to have it called again.
        handleRunObject: function () {
            return 0;       // The object will be called next loop
        },

        // Destruction of the object
        // ---------------------------------------------------------------
        // Called when the object is actually destroyed. This will always be
        // after the main game loop, and out of the actions processing : the 
        // destroy process is queued until the very end of the game loop.
        destroyRunObject: function (bFast) {
        },

        // Displays the object
        // ----------------------------------------------------------------
        // Called when the object needs to be displayed in the browser.
        //    - renderer : the Renderer object which will draw the object
        //    - xDraw : an offset to add to every X coordinate for display
        //    - yDraw : an offset to add to every Y coordinate for display
        // This function will only be called if the object's flags in the C code
        // indicate that this is a displayable object (OEFLAG_SPRITE)
        displayRunObject: function (renderer, xDraw, yDraw) {
            // Example of display of an image, taking the layer and frame position
            // into account
            // var x = this.ho.hoX - this.rh.rhWindowX + this.ho.pLayer.x + xDraw;
            // var y = this.ho.hoY - this.rh.rhWindowY + this.ho.pLayer.y + yDraw;
            // renderer.renderSimpleImage(this.image, x, y, this.image.width, this.image.height, 0, 0);
        },

        // Put the object into pause
        // ----------------------------------------------------------------
        // Called when the game enters pause mode.
        pauseRunObject: function () {
        },

        // Get the object out of pause mode
        // -----------------------------------------------------------------
        // Called when the game quits pause mode.
        continueRunObject: function () {
        },

        // Returns the current font of the object
        // -----------------------------------------------------------------
        // Return value:
        //    The CFontInfo object of the current font used by the object
        // Only called for objects who have a OEFLAG_TEXT flag defined
        getRunObjectFont: function () {
            return null;
        },

        // Sets the current font of the object
        // ------------------------------------------------------------------
        //    - font : a CFontInfo object containing the font to set
        //    - rc : null, or a CRect object containing the new size of the object
        //           it it has to be resized while changing the font
        setRunObjectFont: function (font, rc) {
        },

        // Gets the current color of the text
        // ------------------------------------------------------------------
        // Return value :
        //     A RGB value containing the current color of the font used by the object
        getRunObjectTextColor: function () {
            return 0;
        },

        // Sets the current color of the text
        // ------------------------------------------------------------------
        //     - color : the new color to use to display the text
        setRunObjectTextColor: function (color) {
        },

        // Condition entry
        // -----------------------------------------------------------------
        // Called when a condition of this object is evaluated
        //    - num : the number of the condition, as defined on top of this source
        //    - cnd : a CCndExtension object, allowing you to retrieve the parameters
        //            of the condition
        // Return value :
        //    true or false
        condition: function (num, cnd) {
            // You usually do a switch statement from the "num" parameter
            switch (num) {
                // Dummy condition : true if the parameter is equal to 0
                case CRunFindTheWay.CND_DUMMY:
                    var parameter = cnd.getParamExpression(this.rh, 0);
                    return (parameter == 0);

                // Dummy condition. Example of a condition called from within the object
                // by an action. Returns true if the parameter of the action is 
                // equal to the parameter of the condition.
                case CRunFindTheWay.CND_DUMMY2:
                    var string = cnd.getParamExpString(this.rh, 0);
                    var fromAction = this.rh.rhEvtProg.rhCurParam0;
                    if (string == fromAction)
                        return true;
                    break;
            }
            return false;
        },

        // Action entry
        // --------------------------------------------------------------
        // Called when an action of this object is executed
        //   - num : number of the action, as defined in the list on top of this source
        //   - act : a CActExtension object, allowing you to retrieve the parameters
        //           of the action
        action: function (num, act) {
            switch (num) {
                // Dummy action : changes the position of the object
                case CRunFindTheWay.ACT_DUMMY:
                    var x = act.getParamExpression(this.rh, 0);
                    var y = act.getParamExpression(this.rh, 1);
                    this.setPosition(x, y);
                    break;

                // Dummy action : calls the CND_DUMMY2 condition of this object
                case CRunFindTheWay.ACT_DUMMY2:
                    var string = act.getParamExpString(this.rh, 0);
                    this.generateEvent(CRunFindTheWay.CND_DUMMY2, string);
                    break;
            }
        },

        // Expression entry
        // ------------------------------------------------------------------
        // Called during the evaluation of an expression.
        //    - num : the number of the expression, as defined on top of this source.
        // Note that it is important that your expression function asks for 
        // each and every one of the parameters of the function, each time it is 
        // called. The runtime will crash if you miss parameters.
        // Return value :
        //    - The result of the calculation, a number or a string
        expression: function (num) {
            switch (num) {
                // Dummy expression : adds the two parameters
                case CRunFindTheWay.EXP_DUMMY:
                    var param1 = this.ho.getExpParam();     // Get first parameter
                    var param2 = this.ho.getExpParam();     // Get second parameter
                    return param1 + param2;

                // Dummy expression : returns the length of a string
                case CRunFindTheWay.EXP_DUMMY2:
                    var string = this.ho.getExpParam();     // Get the string parameter
                    return string.length;
            }
            return 0;
        }                                                   // No comma for the last function : the Google compiler
        // we use for final projects does not accept it
    });

// You are perfectly free to define any new class or global function in this code.
// Avoid using generic names, as they may clash with future extensions. The best 
// option is to have a prefix specific to your name or object, inserted before the 
// name of the class or functions.

