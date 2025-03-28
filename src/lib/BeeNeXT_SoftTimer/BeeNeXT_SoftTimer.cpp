#include "../../beenext_config.h"

#if BEENEXT_USE_SOFTTIMER

#include "BeeNeXT_SoftTimer.h"


bool SoftTimer::_ll_inited=false;
ll_t SoftTimer::swtimer_ll;
// ll_t swtimer_ll;

SoftTimer::SoftTimer(){
}

void SoftTimer::init(){
  if(!SoftTimer::_ll_inited){
    // ll_init(&SoftTimer::swtimer_ll, sizeof(swtimer_t));
    ll_init(&swtimer_ll, sizeof(swtimer_t));
    SoftTimer::_ll_inited = true;
  }
}

void SoftTimer::setInterval(unsigned long period_ms, swtimer_cb_t fn, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_INTERVAL, period_ms, fn);
  if(start_first){
    if(fn) fn();
  }
}

void SoftTimer::setInterval(unsigned long period_ms, swtimer_param_cb_t fn, void* param, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_INTERVAL, period_ms, fn, param);
  if(start_first){
    if(fn) fn(param);
  }
}

void SoftTimer::setInterval(unsigned long period_ms, int16_t max_cnt, swtimer_cb_t fn, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_INTERVAL, period_ms, max_cnt, fn);
  if(start_first){
    if(fn) fn();
    swtimer_t* node = SoftTimer::find_swtimer(this->_swtimer_id);
    if(node !=NULL){
      if(node->type == SWTIMER_TYPE_INTERVAL && node->max_cnt > 0) {
        node->max_cnt--;
        if(node->max_cnt == 0) {
          if(node->fn_ready_cb !=NULL){
            node->fn_ready_cb();
          }else
          if(node->fn_ready_param_cb !=NULL){
            node->fn_ready_param_cb(node->param);
          }
          SoftTimer::del_swtimer(node->id);
        }
      }
    }
  }
}

void SoftTimer::setInterval(unsigned long period_ms, int16_t max_cnt, swtimer_param_cb_t fn, void* param, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_INTERVAL, period_ms, fn, param);
  if(start_first){
    if(fn) fn(param);
    swtimer_t* node = SoftTimer::find_swtimer(this->_swtimer_id);
    if(node !=NULL){
      if(node->type == SWTIMER_TYPE_INTERVAL && node->max_cnt > 0) {
        node->max_cnt--;
        if(node->max_cnt == 0) {
          if(node->fn_ready_cb !=NULL){
            node->fn_ready_cb();
          }else
          if(node->fn_ready_param_cb !=NULL){
            node->fn_ready_param_cb(node->param);
          }
          SoftTimer::del_swtimer(node->id);
        }
      }
    }
  }
}

void SoftTimer::ready_cb(swtimer_cb_t fn_ready){
  if(this->_swtimer_id == 0) return;
  swtimer_t* node = SoftTimer::find_swtimer(this->_swtimer_id);
  if(node){
    node->fn_ready_cb = fn_ready;
    node->fn_ready_param_cb = NULL;
  }
}

void SoftTimer::ready_cb(swtimer_param_cb_t fn_ready_param){
  if(this->_swtimer_id == 0) return;
  swtimer_t* node = SoftTimer::find_swtimer(this->_swtimer_id);
  if(node){
    node->fn_ready_cb = NULL;
    node->fn_ready_param_cb = fn_ready_param;
  }
}

void SoftTimer::pause(){
  if(!SoftTimer::_ll_inited) this->init();
  swtimer_t* _cur_node = SoftTimer::find_swtimer(this->_swtimer_id);
  if( _cur_node->type == SWTIMER_TYPE_INTERVAL ){
    _cur_node->pause = true;
  }
}

// ฟังก์ชันใหม่ resume
void SoftTimer::resume(){
  if(!SoftTimer::_ll_inited) this->init();
  swtimer_t* _cur_node = SoftTimer::find_swtimer(this->_swtimer_id);
  if( _cur_node->type == SWTIMER_TYPE_INTERVAL ){
    _cur_node->pause = false;
  }
}


void SoftTimer::delay(unsigned long delay_ms, swtimer_cb_t fn, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_DELAY, delay_ms, fn);
  if(start_first){
    if(fn) fn();
  }
}

void SoftTimer::delay(unsigned long delay_ms, swtimer_param_cb_t fn, void* param, bool start_first){
  if(!SoftTimer::_ll_inited) this->init();
  this->del();
  this->_swtimer_id = SoftTimer::add_swtimer(SWTIMER_TYPE_DELAY, delay_ms, fn, param);
  if(start_first){
    if(fn) fn(param);
  }
}

void SoftTimer::del(){
  if( this->_swtimer_id > 0){
    SoftTimer::del_swtimer(this->_swtimer_id);
    this->_swtimer_id = 0;
  }
}

/******************************************************
 * static member functions
 ******************************************************/
uint16_t SoftTimer::add_swtimer(swtimer_type_t type, unsigned long timeout, swtimer_cb_t fn_cb)
{
  return SoftTimer::add_swtimer(type, timeout, -1, fn_cb);
}

uint16_t SoftTimer::add_swtimer(swtimer_type_t type, unsigned long timeout, swtimer_param_cb_t fn_param_cb, void* param)
{
  return SoftTimer::add_swtimer(type, timeout, -1, fn_param_cb, param);
}

uint16_t SoftTimer::add_swtimer(swtimer_type_t type, unsigned long timeout, int16_t max_cnt, swtimer_cb_t fn_cb){
  swtimer_t* new_node;
  if(ll_is_empty(&swtimer_ll)){
    new_node = (swtimer_t*)  ll_ins_head(&swtimer_ll);
  }else{
    new_node = (swtimer_t*)  ll_ins_tail(&swtimer_ll);
  }

  if(new_node){
    uint16_t id = random(0xFFFF);
    while ( id==0 || SoftTimer::find_swtimer( id ) != NULL ) {
      id = random(0xFFFF);
    }
    new_node->id      = id;
    new_node->type    = type;
    new_node->pause   = false;
    new_node->timeout = timeout;
    new_node->timer   = millis() + new_node->timeout ;
    new_node->max_cnt = max_cnt;
    new_node->fn_cb   = fn_cb;
    new_node->fn_param_cb = NULL;
    new_node->fn_ready_cb = NULL;
    new_node->fn_ready_param_cb = NULL;
    new_node->param   = NULL;
    return new_node->id;
  }
  
  return 0;
}

uint16_t SoftTimer::add_swtimer(swtimer_type_t type, unsigned long timeout, int16_t max_cnt, swtimer_param_cb_t fn_param_cb, void* param){
  swtimer_t* new_node;
  if(ll_is_empty(&swtimer_ll)){
    new_node = (swtimer_t*)  ll_ins_head(&swtimer_ll);
  }else{
    new_node = (swtimer_t*)  ll_ins_tail(&swtimer_ll);
  }

  if(new_node){
    uint16_t id = random(0xFFFF);
    while ( id==0 || SoftTimer::find_swtimer( id ) != NULL ) {
      id = random(0xFFFF);
    }
    new_node->id      = id;
    new_node->type    = type;
    new_node->pause   = false;
    new_node->timeout = timeout;
    new_node->timer   = millis() + new_node->timeout ;
    new_node->max_cnt = -1;
    new_node->fn_cb   = NULL;
    new_node->fn_param_cb = fn_param_cb;
    new_node->fn_ready_cb = NULL;
    new_node->fn_ready_param_cb = NULL;
    new_node->param   = param;
    return new_node->id;
  }
  
  return 0;
}

swtimer_t* SoftTimer::find_swtimer(uint16_t id){
  swtimer_t* node       = (swtimer_t*)  ll_get_head((const ll_t *)&swtimer_ll);
  swtimer_t* node_next  = NULL;
  while(node != NULL) {
    /*'node' might be deleted, so get the next object while 'node' is valid*/
    node_next = (swtimer_t*)ll_get_next(&swtimer_ll, node);

    if(node->id == id) return node;
    
    node = node_next;
  }
  return NULL;
}

bool SoftTimer::del_swtimer(uint16_t id){
  swtimer_t* node   = find_swtimer(id);
  if(node){
    ll_remove(&swtimer_ll, (void *) node);
    free(node);
    return true;
  }
  return false;
}
uint32_t SoftTimer::count_swtimer(){
  return ll_get_len(&swtimer_ll);
}
void SoftTimer::listall_swtimer(){
  Serial.println("--------------------------B");
  swtimer_t* node       = (swtimer_t*)  ll_get_head((const ll_t *)&swtimer_ll);
  swtimer_t* node_next  = NULL;
  while(node != NULL) {
    /*'node' might be deleted, so get the next object while 'node' is valid*/
    node_next = (swtimer_t*)ll_get_next(&swtimer_ll, node);

    Serial.println(node->id);
    
    node = node_next;
  }
  Serial.println("--------------------------E");
}
void SoftTimer::clear_swtimer(){
  ll_clear(&swtimer_ll);
}

void SoftTimer::run(){
  swtimer_t* node       = (swtimer_t*)  ll_get_head((const ll_t *)&swtimer_ll);
  swtimer_t* node_next  = NULL;
  while(node != NULL) {
#if defined(ESP8266) || defined(ESP32)
    ::delay(1);
#endif
    node_next = (swtimer_t*)ll_get_next(&swtimer_ll, node);

    if ( millis() >= node->timer ) {
      node->timer = millis() + node->timeout;

      if( (node->type == SWTIMER_TYPE_DELAY ) || 
          (node->type == SWTIMER_TYPE_INTERVAL && !node->pause && (node->max_cnt > 0 || node->max_cnt == CNT_INFINITY)) )
      {
        if( node->fn_cb)            node->fn_cb();
        else if( node->fn_param_cb) node->fn_param_cb(node->param);

        if(node->type == SWTIMER_TYPE_INTERVAL && node->max_cnt > 0 ) {
          node->max_cnt--;
        }
      }
      
      if ((node->type == SWTIMER_TYPE_DELAY) ||
          (node->type == SWTIMER_TYPE_INTERVAL && node->max_cnt==0)) {
        if(node->fn_ready_cb !=NULL){
          node->fn_ready_cb();
        }else
        if(node->fn_ready_param_cb !=NULL){
          node->fn_ready_param_cb(node->param);
        }
        del_swtimer(node->id);
      }
    }
    
    node = node_next;
  }
}


#endif //#if BEENEXT_USE_SOFTTIMER