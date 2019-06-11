
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
// #include "SDL2/SDL.h"
// #include "SDL2/SDL_image.h"
// #include "SDL2/SDL_ttf.h"
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "context.h"

using namespace std;
/**
 * Inverse square root of two, for normalising velocity
 */
#define REC_SQRT2 0.7071067811865475 


/**
 * Loads the owl texture into the context
 */
int get_owl_texture(context *ctx)
{
  SDL_Surface *image = IMG_Load("assets/owl.png");
  if (!image)
  {
     printf("IMG_Load: %s\n", IMG_GetError());
     return 0;
  }
  ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, image);
  ctx->dest.w = image->w;
  ctx->dest.h = image->h;

  SDL_FreeSurface (image);

  return 1;
}

/**
 * Set the context's text texture to show the text 'text' 
 */
void set_font_text(context *ctx, const char *text)
{
	SDL_Color fg = {0,0,0,255};
    SDL_Surface *text_surface = TTF_RenderText_Blended(ctx->font, text, fg);
    ctx->text_tex = SDL_CreateTextureFromSurface(ctx->renderer, text_surface);
    SDL_FreeSurface(text_surface);
}

/**
 * Load the font we're going to use and set the text to
 * be "Hello owl!"
 */
int get_font_texture(context *ctx)
{
    ctx->font = TTF_OpenFont("assets/FreeSans.ttf", 30);
    
    return 1;
}

/**
 * Processes the input events and sets the velocity
 * of the owl accordingly
 */
void process_input(context *ctx)
{
    


    SDL_Event event;
    

    
    if (ctx->drawing==1){
        set_font_text(ctx, "drawing");
    }else{
        set_font_text(ctx, "NOT drawing");
    }
    while (SDL_PollEvent(&event)) {


    //cout << event.motion.x << endl
    //<< event.motion.y << endl;
        if (event.button.type == SDL_MOUSEBUTTONDOWN){
            if (event.button.button == SDL_BUTTON_LEFT){
                //ctx->drawing = 1;
                //vector<array<int,2>> temp;
                //temp.push_back({event.motion.x,event.motion.y});
                //ctx->points.push_back(temp);
            }
        }else if (event.button.type == SDL_MOUSEBUTTONUP){
            if (event.button.button == SDL_BUTTON_LEFT){
                //ctx->drawing = 0;
            }
        }

    }

}

/**
 * Loop handler that gets called each animation frame,
 * process the input, update the position of the owl and 
 * then render the texture
 */
void loop_handler(void *arg)
{

    context* ctx = static_cast<context*>(arg);

    int vx = 0;
    int vy = 0;
    process_input(ctx);

    ctx->dest.x += ctx->owl_vx;
    ctx->dest.y += ctx->owl_vy;

    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->owl_tex, NULL, &ctx->dest);

    SDL_Rect text_dest = {.x = 50, .y = 175, .w = 0, .h = 0};
    SDL_QueryTexture(ctx->text_tex,
            NULL, NULL, &text_dest.w, &text_dest.h);
    SDL_RenderCopy(ctx->renderer, ctx->text_tex, NULL, &text_dest);
    

    SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    if (ctx->points.size()>0){
        for (auto linija : ctx->points){
            if (linija.size()==1){
                SDL_RenderDrawPoint(ctx->renderer, linija[0][0], linija[0][1]);
            }else{
               for (unsigned i = 0; i<linija.size()-1;++i){
                SDL_RenderDrawLine(ctx->renderer, linija[i][0], linija[i][1],  linija[i+1][0],  linija[i+1][1]);
                } 
            }
               
            
        }
        
    }


    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    SDL_RenderPresent(ctx->renderer);
}

int interpret_charcode_for_keyevent(int eventType, const EmscriptenKeyboardEvent *e)
{
  // Only KeyPress events carry a charCode. For KeyDown and KeyUp events, these don't seem to be present yet, until later when the KeyDown
  // is transformed to KeyPress. Sometimes it can be useful to already at KeyDown time to know what the charCode of the resulting
  // KeyPress will be. The following attempts to do this:
  if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && e->which) return e->which;
  if (e->charCode) return e->charCode;
  if (strlen(e->key) == 1) return (int)e->key[0];
  if (e->which) return e->which;
  return e->keyCode;
}


static inline const char *emscripten_event_type_to_string(int eventType) {
    const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "click", "mousedown", "mouseup", "dblclick", "mousemove", "wheel", "resize", 
        "scroll", "blur", "focus", "focusin", "focusout", "deviceorientation", "devicemotion", "orientationchange", "fullscreenchange", "pointerlockchange", 
        "visibilitychange", "touchstart", "touchend", "touchmove", "touchcancel", "gamepadconnected", "gamepaddisconnected", "beforeunload", 
        "batterychargingchange", "batterylevelchange", "webglcontextlost", "webglcontextrestored", "(invalid)" };
    ++eventType;
    if (eventType < 0) eventType = 0;
    if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
    return events[eventType];
    }

    int number_of_characters_in_utf8_string(const char *str)
{
  if (!str) return 0;
  int num_chars = 0;
  while(*str)
  {
    if ((*str++ & 0xC0) != 0x80) ++num_chars; // Skip all continuation bytes
  }
  return num_chars;
}

int emscripten_key_event_is_printable_character(const EmscriptenKeyboardEvent *keyEvent)
{
  // Not sure if this is correct, but heuristically looks good. Improvements on corner cases welcome.
  return number_of_characters_in_utf8_string(keyEvent->key) == 1;
}

EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
{
    context* ctx = static_cast<context*>(userData);
  int dom_pk_code = emscripten_compute_dom_pk_code(e->code);

  printf("%s, key: \"%s\" %d (printable: %s), code: \"%s\" = %s (%d), location: %lu,%s%s%s%s repeat: %d, locale: \"%s\", char: \"%s\", charCode: %lu (interpreted: %d), keyCode: %s(%lu), which: %lu\n",
    emscripten_event_type_to_string(eventType), e->key, eventType, emscripten_key_event_is_printable_character(e) ? "true" : "false", e->code,
    emscripten_dom_pk_code_to_string(dom_pk_code), dom_pk_code, e->location,
    e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
    e->repeat, e->locale, e->charValue, e->charCode, interpret_charcode_for_keyevent(eventType, e), emscripten_dom_vk_to_string(e->keyCode), e->keyCode, e->which);

  if (eventType == EMSCRIPTEN_EVENT_KEYUP) printf("\n"); // Visual cue
    if(eventType==2 && e->ctrlKey && !e->repeat && e->which==90){
        if (ctx->points.size())
        {
            ctx->drawing=0;
            ctx->points.pop_back();
        }
        
        
    }
  // Return true for events we want to suppress default web browser handling for.
  // For testing purposes, want to return false here on most KeyDown messages so that they get transformed to KeyPress messages.
  return e->keyCode == DOM_VK_BACK_SPACE // Don't navigate away from this test page on backspace.
    || e->keyCode == DOM_VK_TAB // Don't change keyboard focus to different browser UI elements while testing.
    || (e->keyCode >= DOM_VK_F1 && e->keyCode <= DOM_VK_F24) // Don't F5 refresh the test page to reload.
    || e->ctrlKey // Don't trigger e.g. Ctrl-B to open bookmarks
    || e->altKey // Don't trigger any alt-X based shortcuts either (Alt-F4 is not overrideable though)
    || eventType == EMSCRIPTEN_EVENT_KEYPRESS || eventType == EMSCRIPTEN_EVENT_KEYUP; // Don't perform any default actions on these.
}
EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent *e, void *userData)
    {
    context* ctx = static_cast<context*>(userData);

    printf("%s,%ld, screen: (%ld,%ld), client: (%ld,%ld),%s%s%s%s button: %hu, buttons: %hu, movement: (%ld,%ld), canvas: (%ld,%ld), target: (%ld, %ld)\n",
        emscripten_event_type_to_string(eventType), (long)eventType, e->screenX, e->screenY, e->clientX, e->clientY,
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
        e->button, e->buttons, e->movementX, e->movementY, e->canvasX, e->canvasY, e->targetX, e->targetY);

        if(e->buttons==1){
            ctx->drawing=1;
            
        }else{
            ctx->drawing=0;
        }

        switch(eventType){
            case 5:
                if (ctx->drawing){
                    //vector<array<int,2>> temp;
                    //temp.push_back({e->canvasX,e->canvasY});
                    //ctx->points.push_back(temp);
                    ctx->points.push_back({{e->canvasX,e->canvasY}});

                    //ctx->points.pushback(vector<array<int,2>>());
                }
                break;
            case 8:
                if (ctx->drawing){
                    ctx->points[ctx->points.size()-1].push_back({e->canvasX,e->canvasY});
                }
                break;
        }


    return 0;

    }

int main()
{
    SDL_Window *window;
    context ctx;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_CreateWindowAndRenderer(ctx.width, ctx.height, 0, &window, &ctx.renderer);
    SDL_SetRenderDrawColor(ctx.renderer, 255, 255, 255, 255);

    get_owl_texture(&ctx);
    get_font_texture(&ctx);
    ctx.dest.x = 200;
    ctx.dest.y = 100;
    ctx.owl_vx = 0;
    ctx.owl_vy = 0;
    
    emscripten_set_click_callback(0, &ctx, 1, mouse_callback);
    emscripten_set_mousedown_callback(0, &ctx, 1, mouse_callback);
    emscripten_set_mouseup_callback(0, &ctx, 1, mouse_callback);
    emscripten_set_dblclick_callback(0, &ctx, 1, mouse_callback);
    emscripten_set_mousemove_callback(0, &ctx, 1, mouse_callback);
    emscripten_set_keydown_callback(0, &ctx, 1, key_callback);
    emscripten_set_keyup_callback(0, &ctx, 1, key_callback);
    emscripten_set_keypress_callback(0, &ctx, 1, key_callback);

    //ret = emscripten_set_wheel_callback(0, 0, 1, wheel_callback);

    emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);

    return 0;
}
